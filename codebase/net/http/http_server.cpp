#include "http_server.h"
#include "http_server_delegate.h"
#include "http_websocket.h"
#include <mongoose.h>
#include <boost/bind.hpp>
#include <cassert>
#include <cstdio>


namespace codebase {


HttpServer::HttpServer(HttpServerDelegate &delegate)
    : server_(0)
    , delegate_(delegate)
    , running_(false)
    , continue_(false) {
    // nothing
}

HttpServer::~HttpServer() {
    Stop();
}

void HttpServer::Start(int port) {

    if (running_) {
        // TODO(ghilbut): error handling
        return;
    }

	char sport[16];
	sprintf(sport, "%d", port);

    server_ = mg_create_server(this, &HttpServer::ev_handler);
    mg_set_option(server_, "listening_port", sport);

    running_ = true;
    continue_ = true;
    thread_.swap(boost::thread(boost::bind(&HttpServer::run, this)));
}

void HttpServer::Stop() {
    if (!continue_) {
        return;
    }
    continue_ = false;
    thread_.join();
}

void HttpServer::run() {
    while (continue_) {
        mg_poll_server(server_, 1000);
    }
    mg_destroy_server(&server_);
    running_ = false;
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

    delegate_.OnRequest();
    return MG_TRUE;
}

int HttpServer::handle_ws_request(struct mg_connection *conn) {

    if (conn->content_len > 0) {
        if (conn->wsbits & WEBSOCKET_OPCODE_TEXT) {
            HttpWebsocket &ws = ws_table_[conn];
            const std::string text(conn->content, conn->content + conn->content_len);
            delegate_.OnTextMessage(ws, text);
        }
        else {

        }
    }
    return MG_TRUE;
}

int HttpServer::handle_close(struct mg_connection *conn) {

    auto itr = ws_table_.find(conn);
    if (itr != ws_table_.end()) {
        delegate_.OnClose(itr->second);
        ws_table_.erase(itr);
    }
    return MG_TRUE;
}

int HttpServer::handle_ws_connect(struct mg_connection *conn) {
    HttpWebsocket ws(conn);
    ws_table_[conn] = ws;
    delegate_.OnConnect(ws);
    return MG_FALSE;
}

}  // namespace codebase