#include "connection.h"
#include <cassert>


namespace ygg {


NullDelegate<Connection> Connection::kNullDelegate;


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

Connection::Connection(Delegate * delegate)
    : Object(), delegate_(delegate) {
    // nothing
}


}  // namespace ygg
