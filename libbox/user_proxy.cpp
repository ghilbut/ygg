#include "user_proxy.h"

#include "user_info.h"
#include <json/json.h>
#include <cassert>


namespace box {


UserProxy * UserProxy::New(const std::string & json, Session * session) {

    assert(!json.empty());
    assert(session != nullptr);

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
}

const UserInfo * UserProxy::info() const {
    return info_.get();
}

const char * UserProxy::box_id() const {
    return box_id_.c_str();
}

void UserProxy::OnText(Session * session, const std::string & text) {
}

void UserProxy::OnBinary(Session * session, const uint8_t bytes[], size_t size) {
}

void UserProxy::OnClosed(Session * session) {
}

UserProxy::UserProxy(Session * session
                     , const UserInfo::Ptr & info
                     , const std::string & box_id)
    : session_(session), info_(info), box_id_(box_id) {

    assert(session_ != nullptr);
    assert(info_.get() != nullptr);

    session_->BindDelegate(this);
}


}  // namespace box
