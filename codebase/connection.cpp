#include "connection.h"
#include <cassert>

namespace codebase {


class NullDelegate : public Connection::Delegate {
public:
    virtual void OnText(Connection*, const std::string&) {}
    virtual void OnBinary(Connection*, const std::vector<uint8_t>&) {}
    virtual void OnClosed(Connection*) {}
};

static NullDelegate kNullDelegate;


void Connection::FireOnTextEvent(const std::string & text) {
    delegate_->OnText(this, text);
}

void Connection::FireOnBinaryEvent(const std::vector<uint8_t> & bytes) {
    delegate_->OnBinary(this, bytes);
}

void Connection::FireOnClosedEvent() {
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
    : Object(this), delegate_(&kNullDelegate) {
    // nothing
}


}  // namespace codebase
