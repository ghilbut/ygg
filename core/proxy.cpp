#include "user_proxy.h"

#include <json/json.h>
#include <cassert>


namespace ygg
namespace core {


Proxy::Ptr Proxy::New(Connection::Ptr &  conn, const std::string & json) {

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

    return new Proxy(conn, info, box_id);
}

Proxy::~Proxy() {
    Close();
}

size_t Proxy::SendText(const std::string & text) const {
    return conn_->SendText(text);
}

size_t Proxy::SendBinary(const std::vector<uint8_t> & bytes) const {
    return SendBinary(bytes);
}

void Proxy::Close() {
    if (conn_ != nullptr) {
        conn_->Close();
    }
}

void Proxy::BindDelegate(Delegate * delegate) {
    assert(delegate != nullptr);
    delegate_ = delegate;
}

void Proxy::UnbindDelegate() {
    delegate_ = &kNullDelegate;
}

const UserDesc & Proxy::info() const {
    return *info_;
}

const char * Proxy::box_id() const {
    return box_id_.c_str();
}

void Proxy::OnText(Connection * conn, const std::string & text) {
    assert(conn != nullptr);
    assert(conn == conn_);
    delegate_->OnText(this, text);
}

void Proxy::OnBinary(Connection * conn, const std::vector<uint8_t> & bytes) {
    assert(conn != nullptr);
    assert(conn == conn_);
    delegate_->OnBinary(this, bytes);
}

void Proxy::OnClosed(Connection * conn) {
    assert(conn != nullptr);
    assert(conn == conn_);

    conn_->UnbindDelegate();
    conn_ = nullptr;
    delegate_->OnClosed(this);
}

Proxy::Proxy(Connection::Ptr & conn
                     , const UserDesc::Ptr & info
                     , const std::string & box_id)
    : Object()
    , delegate_(&kNullDelegate)
    , conn_(conn)
    , info_(info)
    , box_id_(box_id) {

    assert(conn_ != nullptr);
    assert(info_.get() != nullptr);

    conn_->BindDelegate(this);
}


}  // namespace core
}  // namespace ygg
