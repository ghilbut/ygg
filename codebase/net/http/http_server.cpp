#include "http_server.h"
#include "http_server_delegate.h"
#include "http_server_null_delegate.h"
#include "http_websocket.h"
#include <mongoose.h>
#include <boost/bind.hpp>
#include <cassert>
#include <cstdio>



namespace codebase {


HttpServer::NullDelegate * const HttpServer::kNullDelegate_ = HttpServer::NewNullDelegate();

HttpServer::NullDelegate *HttpServer::NewNullDelegate() {
	HttpServer::NullDelegate* deleagate = new HttpServer::NullDelegate();
	atexit(&HttpServer::DelNullDelegate);
	return deleagate;
}

void HttpServer::DelNullDelegate() {
	delete kNullDelegate_;
}


HttpServer::HttpServer(HttpServerDelegate *delegate)
	: server_(0)
	, delegate_(delegate == nullptr ? kNullDelegate_ : delegate)
	, is_running_(false)
	, is_stopping_(false)
	, is_stopped_(true) {
	// nothing
}

HttpServer::~HttpServer() {
    Stop();
}

void HttpServer::Start(int port) {

    if (is_stopping_ || !is_stopped_) {
        // TODO(ghilbut): error handling
        return;
    }

	is_running_ = false;
	is_stopping_ = false;
	is_stopped_ = false;

	char sport[16];
	sprintf(sport, "%d", port);

    server_ = mg_create_server(this, &HttpServer::ev_handler);
    mg_set_option(server_, "listening_port", sport);

    thread_.swap(boost::thread(boost::bind(&HttpServer::run, this)));

	std::unique_lock<std::mutex> lock(mutex_);
	cv_.wait(lock, [this]() { return (bool) is_running_; });
}

void HttpServer::Stop() {

	if (is_stopping_ || is_stopped_) {
		return;
	}

	is_stopping_ = true;
	thread_.join();
	is_stopping_ = false;
	is_stopped_ = true;
}

bool HttpServer::IsRunning() const {
	return is_running_;
}

bool HttpServer::IsStopping() const {
	return is_stopped_;
}

bool HttpServer::IsStopped() const {
	return is_stopped_;
}


void HttpServer::run() {

	{
		std::lock_guard<std::mutex> lock(mutex_);
		is_running_ = true;
	}
	cv_.notify_one();

    while (!is_stopping_) {
        mg_poll_server(server_, 1000);
    }
    mg_destroy_server(&server_);
    is_running_ = false;
}

int HttpServer::ev_handler(struct mg_connection *conn, enum mg_event ev) {

    HttpServer *self = static_cast<HttpServer*>(conn->server_param);

    if (ev == MG_REQUEST) {
        if (conn->is_websocket) {
            return self->handle_ws_request(conn);
        }
        else {
            return self->handle_request(conn);
        }
    }
    if (ev == MG_WS_CONNECT) {
        return self->handle_ws_connect(conn);
    }
    if (ev == MG_CLOSE) {
        return self->handle_close(conn);
    }
    if (ev == MG_AUTH) {
        return self->handle_auth(conn);
    }
    return MG_FALSE;
}

int HttpServer::handle_auth(struct mg_connection *conn) {
    return MG_TRUE;
}

int HttpServer::handle_request(struct mg_connection *conn) {

    delegate_->OnRequest();
    return MG_TRUE;
}

int HttpServer::handle_ws_request(struct mg_connection *conn) {

    if (conn->content_len > 0) {
        if (conn->wsbits & WEBSOCKET_OPCODE_TEXT) {
            HttpWebsocket &ws = ws_table_[conn];
            const std::string text(conn->content, conn->content + conn->content_len);
            delegate_->OnTextMessage(ws, text);
        }
        else {

        }
    }
    return MG_TRUE;
}

int HttpServer::handle_close(struct mg_connection *conn) {

    auto itr = ws_table_.find(conn);
    if (itr != ws_table_.end()) {
        delegate_->OnClose(itr->second);
        ws_table_.erase(itr);
    }
    return MG_TRUE;
}

int HttpServer::handle_ws_connect(struct mg_connection *conn) {
    HttpWebsocket ws(conn);
    ws_table_[conn] = ws;
    delegate_->OnConnect(ws);
    return MG_FALSE;
}

}  // namespace codebase