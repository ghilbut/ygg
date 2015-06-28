#include "box_proxy.h"

#include <cassert>


namespace box {


class NullDelegate : public BoxProxy::Delegate {
public:
    virtual void OnText(BoxProxy*, const std::string&) {}
    virtual void OnBinary(BoxProxy*, const std::vector<uint8_t>&) {}
    virtual void OnClosed(BoxProxy*) {}
};

static NullDelegate kNullDelegate;


BoxProxy::Ptr BoxProxy::New(Connection::Ptr & conn, const std::string & json) {

    assert(conn != nullptr);
    assert(!json.empty());

    BoxDesc::Ptr info(BoxDesc::New(json));
    if (info == nullptr) {
        return nullptr;
    }

    return new BoxProxy(conn, info);
}

BoxProxy::~BoxProxy() {
}

void BoxProxy::BindDelegate(Delegate * delegate) {
    assert(delegate != nullptr);
    delegate_ = delegate;
}

void BoxProxy::UnbindDelegate() {
    delegate_ = &kNullDelegate;
}

const BoxDesc & BoxProxy::info() const {
    return *info_;
}

size_t BoxProxy::SendText(const std::string & text) const {
    assert(conn_ != nullptr);
    return conn_->SendText(text);
}

size_t BoxProxy::SendBinary(const std::vector<uint8_t> & bytes) const {
    assert(conn_ != nullptr);
    return conn_->SendBinary(bytes);
}

void BoxProxy::Close() {
    assert(conn_ != nullptr);
    conn_->Close();
}

void BoxProxy::OnText(Connection * conn, const std::string & text) {
    assert(conn == conn_);
    delegate_->OnText(this, text);
}

void BoxProxy::OnBinary(Connection * conn, const std::vector<uint8_t> & bytes) {
    assert(conn == conn_);
    delegate_->OnBinary(this, bytes);
}

void BoxProxy::OnClosed(Connection * conn) {
    assert(conn == conn_);
    delegate_->OnClosed(this);
}

BoxProxy::BoxProxy(Connection::Ptr & conn, const BoxDesc::Ptr & info)
    : Object(this)
    , delegate_(&kNullDelegate)
    , conn_(conn)
    , info_(info) {

    assert(conn_ != nullptr);
    assert(info_ != nullptr);

    conn_->BindDelegate(this);
}


}  // namespace box
