#include "session.h"
#include "session_delegate.h"
#include <cassert>


namespace codebase {


class NullDelegate : public Session::Delegate {
public:
    void OnText(Session*, const std::string&) {}
    void OnBinary(Session*, const std::vector<uint8_t>&) {}
    void OnClosed(Session*) {}
};

static NullDelegate kNullDelegate;


Session::Session()
    : Object(), delegate_(&kNullDelegate) {
    // nothing
}

void Session::FireOnTextEvent(const std::string & text) {
    delegate_->OnText(this, text);
}

void Session::FireOnBinaryEvent(const std::vector<uint8_t> & bytes) {
    delegate_->OnBinary(this, bytes);
}

void Session::FireOnClosedEvent() {
    delegate_->OnClosed(this);
}

void Session::BindDelegate(Delegate * delegate) {
    assert(delegate != nullptr);
    delegate_ = delegate;
}

void Session::UnbindDelegate() {
    delegate_ = &kNullDelegate;
}


}  // namespace codebase
