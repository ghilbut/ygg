#include "http/http_websocket.h"
#include "http_websocket_impl.h"
#include <mongoose.h>


HttpWebsocket::HttpWebsocket(const HttpWebsocket& other)
    : impl_(other.impl_) {
    // nothing
}
HttpWebsocket::HttpWebsocket(struct mg_connection* conn)
    : impl_(new Impl(conn)) {
    // nothing
}

HttpWebsocket& HttpWebsocket::operator= (const HttpWebsocket& other) {
    impl_ = other.impl_;
    return *this;
}

bool HttpWebsocket::operator== (const HttpWebsocket& other) const {
    return (impl_ == other.impl_);
}

bool HttpWebsocket::operator!= (const HttpWebsocket& other) const {
    return (impl_ != other.impl_);
}

bool HttpWebsocket::operator< (const HttpWebsocket& other) const {
    return (impl_ < other.impl_);
}

bool HttpWebsocket::IsNull() const {
    return (impl_ == nullptr);
}

void HttpWebsocket::WriteText(const std::string& text) const {
    if (impl_ != nullptr) {
        impl_->WriteText(text);
    }
}

void HttpWebsocket::Close() {
    if (impl_ != nullptr) {
        impl_->Close();
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