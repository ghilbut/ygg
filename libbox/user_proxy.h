#ifndef YGG_LIBBOX_USER_PROXY_H_
#define YGG_LIBBOX_USER_PROXY_H_

#include "codebase/session_delegate.h"
#include "codebase/user_desc.h"


using namespace codebase;


namespace box {


class UserProxy
    : public Object<UserProxy>
    , public Session::Delegate {

public:
    class Delegate {
    public:
        virtual void OnText(UserProxy * user, const std::string & text) = 0;
        virtual void OnBinary(UserProxy * user, const std::vector<uint8_t> & bytes) = 0;
        virtual void OnClosed(UserProxy * user) = 0;
    };

public:
    static UserProxy::Ptr New(Session::Ptr & session, const std::string & json);
    ~UserProxy();

    size_t SendText(const std::string & text) const;
    size_t SendBinary(const std::vector<uint8_t> & bytes) const;
    void Close();

    void BindDelegate(Delegate * delegate);
    void UnbindDelegate();

    const UserDesc & info() const;
    const char * box_id() const;

    // net::Session::Delegate
    virtual void OnText(Session * session, const std::string & text);
    virtual void OnBinary(Session * session, const std::vector<uint8_t> & bytes);
    virtual void OnClosed(Session * session);

private:
    UserProxy(
        Session::Ptr & session
        , const UserDesc::Ptr & info
        , const std::string & box_id);

private:
    Delegate * delegate_;
    Session::Ptr session_;
    const UserDesc::Ptr info_;
    const std::string box_id_;
};


}  // namespace box


#endif  // YGG_LIBBOX_USER_PROXY_H_
