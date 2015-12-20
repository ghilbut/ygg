#include "connection.h"
#include <cassert>


namespace ygg {


class NullDelegate : public Connection::Delegate {
public:
    virtual void OnText(Connection*, const Text&) {}
    virtual void OnBinary(Connection*, const Bytes&) {}
    virtual void OnClosed(Connection*) {}
};

static NullDelegate kNullDelegate;



void Connection::FireOnTextEvent(const Text & text) {
    assert(delegate_ != nullptr);
    delegate_->OnText(this, text);
}

void Connection::FireOnBinaryEvent(const Bytes & bytes) {
    assert(delegate_ != nullptr);
    delegate_->OnBinary(this, bytes);
}

void Connection::FireOnClosedEvent() {
    assert(delegate_ != nullptr);
    delegate_->OnClosed(this);
}

void Connection::BindDelegate(Delegate * delegate) {
    assert(delegate != nullptr);
    delegate_ = delegate;
}

void Connection::UnbindDelegate() {
    delegate_ = &kNullDelegate;
}

Connection::Connection()
    : Object(), delegate_(&kNullDelegate) {
    // nothing
}


}  // namespace ygg
