#ifndef YGG_LIBBOX_USER_PROXY_H_
#define YGG_LIBBOX_USER_PROXY_H_

#include "user_info.h"
#include "codebase/session_delegate.h"


using namespace codebase;


namespace box {


class UserProxy : public Session::Delegate {
public:
    static UserProxy * New(const std::string & json, Session * session);
    ~UserProxy();

    const UserInfo * info() const;

    // net::Session::Delegate
    virtual void OnText(Session * session, const std::string & text);
    virtual void OnBinary(Session * session, const uint8_t bytes[], size_t size);
    virtual void OnClosed(Session * session);

private:
    UserProxy(const UserInfo::Ptr & info, Session * session);

private:
    const UserInfo::Ptr info_;
    Session * session_;
};


}  // namespace box


#endif  // YGG_LIBBOX_USER_PROXY_H_
