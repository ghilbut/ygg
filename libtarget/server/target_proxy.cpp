#include "target_proxy.h"
#include <cassert>


namespace ygg {
namespace target {
namespace server {



class NullTargetProxyDelegate : public TargetProxy::Delegate {
public:
    virtual void OnText(TargetProxy*, const Text&) {}
    virtual void OnBinary(TargetProxy*, const Bytes&) {}
    virtual void OnClosed(TargetProxy*) {}
};

static NullTargetProxyDelegate kNullDelegate;



TargetProxy::Ptr TargetProxy::New(Connection::Ptr & conn,
                                  TargetDesc::Ptr & desc) {
    assert(conn != nullptr);
    assert(desc != nullptr);
    return new TargetProxy(conn, desc);
}


TargetProxy::~TargetProxy() {
    // nothing
}


void TargetProxy::BindDelegate(Delegate * delegate) {
    assert(delegate != nullptr);
    delegate_ = delegate;
}


void TargetProxy::UnbindDelegate() {
    delegate_ = &kNullDelegate;
}


void TargetProxy::OnText(Connection * conn, const Text & text) {
    assert(conn != nullptr);
    delegate_->OnText(this, text);
}


void TargetProxy::OnBinary(Connection * conn, const Bytes & bytes) {
    assert(conn != nullptr);
    delegate_->OnBinary(this, bytes);
}


void TargetProxy::OnClosed(Connection * conn) {
    assert(conn != nullptr);
    delegate_->OnClosed(this);
}

TargetProxy::TargetProxy(Connection::Ptr & conn, TargetDesc::Ptr & desc) 
    : delegate_(&kNullDelegate), conn_(conn), desc_(desc) {

    conn_->BindDelegate(this);
}


}  // namespace server
}  // namespace target
}  // namespace ygg
