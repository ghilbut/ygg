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


UserProxy::Ptr UserProxy::New(Session::Ptr &  session, const std::string & json) {

    assert(session != nullptr);
    assert(!json.empty());

    Json::Value root;
    UserInfo::Ptr info(UserInfo::New(json, root));
    if (info == nullptr) {
        return nullptr;
    }

    Json::Value tmp = root["box-id"];
    if (!tmp.isString()) {
        return nullptr;
    }
    const std::string box_id(tmp.asString());

    return new UserProxy(session, info, box_id);
}

UserProxy::~UserProxy() {
    Close();
}

size_t UserProxy::SendText(const std::string & text) const {
    return session_->SendText(text);
}

size_t UserProxy::SendBinary(const std::vector<uint8_t> & bytes) const {
    return SendBinary(bytes);
}

void UserProxy::Close() {
    if (session_ != nullptr) {
        session_->Close();
    }
}

void UserProxy::BindDelegate(Delegate * delegate) {
    assert(delegate != nullptr);
    delegate_ = delegate;
}

void UserProxy::UnbindDelegate() {
    delegate_ = &kNullDelegate;
}

const UserInfo & UserProxy::info() const {
    return *info_;
}

const char * UserProxy::box_id() const {
    return box_id_.c_str();
}

void UserProxy::OnText(Session * session, const std::string & text) {
    assert(session != nullptr);
    assert(session == session_);
    delegate_->OnText(this, text);
}

void UserProxy::OnBinary(Session * session, const std::vector<uint8_t> & bytes) {
    assert(session != nullptr);
    assert(session == session_);
    delegate_->OnBinary(this, bytes);
}

void UserProxy::OnClosed(Session * session) {
    assert(session != nullptr);
    assert(session == session_);

    session_->UnbindDelegate();
    session_ = nullptr;
    delegate_->OnClosed(this);
}

UserProxy::UserProxy(Session::Ptr & session
                     , const UserInfo::Ptr & info
                     , const std::string & box_id)
    : Object(this)
    , delegate_(&kNullDelegate)
    , session_(session)
    , info_(info)
    , box_id_(box_id) {

    assert(session_ != nullptr);
    assert(info_.get() != nullptr);

    session_->BindDelegate(this);
}


}  // namespace box
