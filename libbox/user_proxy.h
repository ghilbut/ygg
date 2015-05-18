#ifndef YGG_LIBBOX_USER_PROXY_H_
#define YGG_LIBBOX_USER_PROXY_H_

#include "user_info.h"
#include "codebase/session_delegate.h"


using namespace codebase;


namespace box {


class UserProxy : 
    public Object<UserProxy>
    , public Session::Delegate {

public:
    static UserProxy::Ptr New(Session::Ptr & session, const std::string & json);
    ~UserProxy();

    const UserInfo * info() const;
    const char * box_id() const;

    // net::Session::Delegate
    virtual void OnText(Session * session, const std::string & text);
    virtual void OnBinary(Session * session, const uint8_t bytes[], size_t size);
    virtual void OnClosed(Session * session);

private:
    UserProxy(
        Session::Ptr & session
        , const UserInfo::Ptr & info
        , const std::string & box_id);

private:
    Session::Ptr session_;
    const UserInfo::Ptr info_;
    const std::string box_id_;
};


}  // namespace box


#endif  // YGG_LIBBOX_USER_PROXY_H_
