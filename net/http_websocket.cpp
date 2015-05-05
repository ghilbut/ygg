#include "http_websocket.h"

#include "http_websocket_impl.h"
#include <cassert>


namespace net {
namespace http {


WebSocket::WebSocket(Impl * pimpl)
    : pimpl_(pimpl) {
    
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
    pimpl_->BindDelegate(delegate);
}

void WebSocket::UnbindDelegate() {
    pimpl_->UnbindDelegate();
}

void WebSocket::FireOnTextMessageEvent(const std::string & text) {
    pimpl_->FireOnTextMessageEvent(text);
}

void WebSocket::FireOnClosedEvent() {
    pimpl_->FireOnClosedEvent();
}


}  // namespace http
}  // namespace net
