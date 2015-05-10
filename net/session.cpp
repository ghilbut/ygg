#include "session.h"
#include "session_delegate.h"
#include <cassert>


namespace net {


class NullDelegate : public Session::Delegate {
public:
    void OnText(Session * session, const std::string & text) {}
    void OnBinary(Session * session, const uint8_t bytes[], size_t size) {}
    void OnClosed(Session * session) {}
};

static NullDelegate g_null_delegate;


void Session::FireOnTextEvent(const std::string & text) {
    delegate_->OnText(this, text);
}

void Session::FireOnBinaryEvent(const uint8_t bytes[], size_t size) {
    delegate_->OnBinary(this, bytes, size);
}

void Session::FireOnClosedEvent() {
    delegate_->OnClosed(this);
}

void Session::BindDelegate(Delegate * delegate) {
    assert(delegate != nullptr);
    delegate_ = delegate;
}

void Session::UnbindDelegate() {
    delegate_ = &g_null_delegate;
}


}  // namespace net
