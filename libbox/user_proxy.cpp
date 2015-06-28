#include "user_proxy.h"

#include <json/json.h>
#include <cassert>


namespace box {


class NullDelegate : public UserProxy::Delegate {
public:
    virtual void OnText(UserProxy*, const std::string&) {}
    virtual void OnBinary(UserProxy*, const std::vector<uint8_t>&) {}
    virtual void OnClosed(UserProxy*) {}
};

static NullDelegate kNullDelegate;


UserProxy::Ptr UserProxy::New(Connection::Ptr &  conn, const std::string & json) {

    assert(conn != nullptr);
    assert(!json.empty());

    Json::Value root;
    UserDesc::Ptr info(UserDesc::New(json, root));
    if (info == nullptr) {
        return nullptr;
    }

    Json::Value tmp = root["box-id"];
    if (!tmp.isString()) {
        return nullptr;
    }
    const std::string box_id(tmp.asString());

    return new UserProxy(conn, info, box_id);
}

UserProxy::~UserProxy() {
    Close();
}

size_t UserProxy::SendText(const std::string & text) const {
    return conn_->SendText(text);
}

size_t UserProxy::SendBinary(const std::vector<uint8_t> & bytes) const {
    return SendBinary(bytes);
}

void UserProxy::Close() {
    if (conn_ != nullptr) {
        conn_->Close();
    }
}

void UserProxy::BindDelegate(Delegate * delegate) {
    assert(delegate != nullptr);
    delegate_ = delegate;
}

void UserProxy::UnbindDelegate() {
    delegate_ = &kNullDelegate;
}

const UserDesc & UserProxy::info() const {
    return *info_;
}

const char * UserProxy::box_id() const {
    return box_id_.c_str();
}

void UserProxy::OnText(Connection * conn, const std::string & text) {
    assert(conn != nullptr);
    assert(conn == conn_);
    delegate_->OnText(this, text);
}

void UserProxy::OnBinary(Connection * conn, const std::vector<uint8_t> & bytes) {
    assert(conn != nullptr);
    assert(conn == conn_);
    delegate_->OnBinary(this, bytes);
}

void UserProxy::OnClosed(Connection * conn) {
    assert(conn != nullptr);
    assert(conn == conn_);

    conn_->UnbindDelegate();
    conn_ = nullptr;
    delegate_->OnClosed(this);
}

UserProxy::UserProxy(Connection::Ptr & conn
                     , const UserDesc::Ptr & info
                     , const std::string & box_id)
    : Object(this)
    , delegate_(&kNullDelegate)
    , conn_(conn)
    , info_(info)
    , box_id_(box_id) {

    assert(conn_ != nullptr);
    assert(info_.get() != nullptr);

    conn_->BindDelegate(this);
}


}  // namespace box
