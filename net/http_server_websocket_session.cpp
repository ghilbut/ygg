#include "http_websocket.h"

#include "http_websocket_delegate.h"
#include "http_websocket_impl.h"
#include <cassert>


namespace net {
namespace http {


WebSocket::WebSocket(Impl * pimpl)
    : pimpl_(pimpl), delegate_(nullptr) {
    assert(pimpl_ != nullptr);
}

WebSocket::WebSocket(mg_connection * conn)
    : pimpl_(new Impl(conn)) {
    assert(conn != nullptr);
}

WebSocket::~WebSocket() {
    delete pimpl_;
}

size_t WebSocket::SendTextMessage(const std::string & text) const {
    return pimpl_->SendTextMessage(text);
}

size_t WebSocket::SendBinaryMessage(const uint8_t bytes[], size_t size) const {
    return pimpl_->SendBinaryMessage(bytes, size);
}

void WebSocket::BindDelegate(Delegate * delegate) {
    delegate_ = delegate;
}

void WebSocket::UnbindDelegate() {
    delegate_ = nullptr;
}

void WebSocket::FireOnTextMessageEvent(const std::string & text) {
    if (delegate_ != nullptr) {
        delegate_->OnTextMessage(this, text);
    }
}

void WebSocket::FireOnClosedEvent() {
    if (delegate_ != nullptr) {
        delegate_->OnClosed(this);
    }
}


}  // namespace http
}  // namespace net
