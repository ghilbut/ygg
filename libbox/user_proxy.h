#ifndef YGG_LIBBOX_USER_PROXY_H_
#define YGG_LIBBOX_USER_PROXY_H_

#include "net/session_delegate.h"


using namespace net;


namespace box {


class UserInfo;


class UserProxy : public net::Session::Delegate {
public:
    static UserProxy * New(const std::string & json, Session * session);
    ~UserProxy();

    const UserInfo * info() const;

    // net::Session::Delegate
    virtual void OnText(Session * session, const std::string & text);
    virtual void OnBinary(Session * session, const uint8_t bytes[], size_t size);
    virtual void OnClosed(Session * session);

private:
    UserProxy(const UserInfo * info, Session * session);

private:
    const UserInfo * info_;
    net::Session * session_;
};


}  // namespace box


#endif  // YGG_LIBBOX_USER_PROXY_H_
