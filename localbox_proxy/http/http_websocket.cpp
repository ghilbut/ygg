#include "http_websocket.h"
#include <mongoose.h>


HttpWebsocket::HttpWebsocket(struct mg_connection* conn)
    : conn_(conn) {
    // nothing
}

void HttpWebsocket::WriteText(const std::string& text) const {
    if (!text.empty()) {
        mg_websocket_write(conn_, WEBSOCKET_OPCODE_TEXT, text.c_str(), text.length());
    }
}

HttpWebsocket::~HttpWebsocket() {
	mg_websocket_write(conn_, WEBSOCKET_OPCODE_CONNECTION_CLOSE, 0, 0);
}