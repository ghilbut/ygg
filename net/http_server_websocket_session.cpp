#include "http_server_websocket_session.h"

#include "http_server_websocket_session_delegate.h"
#include "http_server_websocket_session_impl.h"
#include <cassert>


namespace net {
namespace http {
namespace server {
namespace websocket {


Session::Session(Impl * pimpl)
    : pimpl_(pimpl), delegate_(nullptr) {
    assert(pimpl_ != nullptr);
}

Session::Session(mg_connection * conn)
    : pimpl_(new Impl(conn)) {
    assert(conn != nullptr);
}

Session::~Session() {
    delete pimpl_;
}

size_t Session::SendTextMessage(const std::string & text) const {
    return pimpl_->SendTextMessage(text);
}

size_t Session::SendBinaryMessage(const uint8_t bytes[], size_t size) const {
    return pimpl_->SendBinaryMessage(bytes, size);
}

void Session::Close() {
    pimpl_->Close();
}

void Session::BindDelegate(Delegate * delegate) {
    delegate_ = delegate;
}

void Session::UnbindDelegate() {
    delegate_ = nullptr;
}

void Session::FireOnTextMessageEvent(const std::string & text) {
    if (delegate_ != nullptr) {
        delegate_->OnTextMessage(this, text);
    }
}

void Session::FireOnClosedEvent() {
    if (delegate_ != nullptr) {
        delegate_->OnClosed(this);
    }
}


}  // namespace websocket
}  // namespace server
}  // namespace http
}  // namespace net
