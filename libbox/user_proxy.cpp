#include "user_proxy.h"

#include "user_info.h"
#include <cassert>


namespace box {


UserProxy * UserProxy::New(const std::string & json, Session * session) {

    assert(!json.empty());
    assert(session != nullptr);

    UserInfo::Ptr info(UserInfo::New(json));
    if (info == nullptr) {
        return nullptr;
    }

    return new UserProxy(info, session);
}

UserProxy::~UserProxy() {
}

const UserInfo * UserProxy::info() const {
    return info_.get();
}

void UserProxy::OnText(Session * session, const std::string & text) {
}

void UserProxy::OnBinary(Session * session, const uint8_t bytes[], size_t size) {
}

void UserProxy::OnClosed(Session * session) {
}

UserProxy::UserProxy(const UserInfo::Ptr & info, Session * session)
    : info_(info), session_(session) {

    assert(info_.get() != nullptr);
    assert(session_ != nullptr);
    session_->BindDelegate(this);
}


}  // namespace box