#include "http_server.h"
#include "http_server_delegate.h"
#include "http_websocket.h"
#include <mongoose.h>
#include <boost/bind.hpp>
#include <cassert>


HttpServer::HttpServer(HttpServerDelegate& delegate)
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

    server_ = mg_create_server(this, &HttpServer::ev_handler);
    mg_set_option(server_, "listening_port", "8070");

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

    HttpServer* self = static_cast<HttpServer*>(conn->server_param);
    HttpServerDelegate& delegate = self->delegate_;
    WSTable& ws_table = self->ws_table_;

    switch (ev) {
    case MG_REQUEST:
    {
        if (conn->is_websocket) {

            if (conn->content_len > 0) {
                if (conn->wsbits & WEBSOCKET_OPCODE_TEXT) {
                    HttpWebsocket* ws = ws_table[conn];
                    const std::string text(conn->content, conn->content + conn->content_len);
                    delegate.OnTextMessage(ws, text);
                }
                else {

                }
            }
        }
        else {
            // handle_request(conn);

            delegate.OnRequest();
        }
        return MG_TRUE;
    }
    case MG_WS_CONNECT:
    {
        HttpWebsocket *ws = new HttpWebsocket(conn);
        ws_table[conn] = ws;
        delegate.OnConnect(ws);
        return MG_FALSE;
    }
    case MG_CLOSE:
    {
        HttpWebsocket *ws = ws_table[conn];
        delegate.OnClose(ws);
        delete ws;
        return MG_TRUE;
    }
    case MG_AUTH:
        return MG_TRUE;
    default:
        return MG_FALSE;
    }
}