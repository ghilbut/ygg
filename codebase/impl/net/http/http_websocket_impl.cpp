#include "net/http/http_websocket.h"
#include "http_websocket_impl.h"
#include <mongoose.h>


namespace codebase {


HttpWebsocket::HttpWebsocket(struct mg_connection* conn)
    : Object(new Impl(conn)) {
    // nothing
}

void HttpWebsocket::WriteText(const std::string& text) const {
	Impl* pimpl = static_cast<Impl*>(impl_.get());
    if (pimpl != nullptr) {
        pimpl->WriteText(text);
    }
}

void HttpWebsocket::Close() {
	Impl* pimpl = static_cast<Impl*>(impl_.get());
    if (pimpl != nullptr) {
        pimpl->Close();
    }
}



HttpWebsocket::Impl::Impl(struct mg_connection* conn)
    : conn_(conn) {
    // nothing
}

HttpWebsocket::Impl::~Impl() {
    Close();
}

void HttpWebsocket::Impl::WriteText(const std::string& text) const {
    if (conn_ != nullptr && !text.empty()) {
        mg_websocket_write(conn_, WEBSOCKET_OPCODE_TEXT, text.c_str(), text.length());
    }
}

void HttpWebsocket::Impl::Close() {
    if (conn_ != nullptr) {
        mg_websocket_write(conn_, WEBSOCKET_OPCODE_CONNECTION_CLOSE, 0, 0);
        conn_ = nullptr;
    }
}


}  // namespace codebase