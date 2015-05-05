#include "http_websocket_impl.h"

#include "http_websocket_delegate.h"
#include <mongoose.h>


namespace net {
namespace http {


WebSocket::Impl::Impl(struct mg_connection * conn)
    : conn_(conn), delegate_(nullptr) {
}

WebSocket::Impl::~Impl() {
}

size_t WebSocket::Impl::SendTextMessage(const std::string & text) const {
    return mg_websocket_write(conn_, WEBSOCKET_OPCODE_TEXT, text.c_str(), text.length());
}

size_t WebSocket::Impl::SendBinaryMessage(const uint8_t bytes[], size_t size) const {
    const char * data = reinterpret_cast<const char *>(bytes);
    return mg_websocket_write(conn_, WEBSOCKET_OPCODE_BINARY, data, size);
}

void WebSocket::Impl::BindDelegate(Delegate * delegate) {
    delegate_ = delegate;
}

void WebSocket::Impl::UnbindDelegate() {
    delegate_ = nullptr;
}

void WebSocket::Impl::FireOnTextMessageEvent(const std::string & text) {
    if (delegate_ != nullptr) {
        delegate_->OnTextMessage(text);
    }
}

void WebSocket::Impl::FireOnClosedEvent() {
    if (delegate_ != nullptr) {
        delegate_->OnClosed();
    }
}


}  // namespace net
}  // namespace http
