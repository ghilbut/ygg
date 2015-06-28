#ifndef YGG_CODEBASE_USER_PROXY_H_
#define YGG_CODEBASE_USER_PROXY_H_

#include "connection.h"
#include "user_desc.h"


namespace codebase {


class UserProxy
    : public Object<UserProxy>
    , public Connection::Delegate {

public:
    class Delegate {
    public:
        virtual void OnText(UserProxy * user, const std::string & text) = 0;
        virtual void OnBinary(UserProxy * user, const std::vector<uint8_t> & bytes) = 0;
        virtual void OnClosed(UserProxy * user) = 0;
    };

public:
    static UserProxy::Ptr New(Connection::Ptr & conn, const std::string & json);
    ~UserProxy();

    size_t SendText(const std::string & text) const;
    size_t SendBinary(const std::vector<uint8_t> & bytes) const;
    void Close();

    void BindDelegate(Delegate * delegate);
    void UnbindDelegate();

    const UserDesc & info() const;
    const char * box_id() const;

    // codebase::Connection::Delegate
    virtual void OnText(Connection * conn, const std::string & text);
    virtual void OnBinary(Connection * conn, const std::vector<uint8_t> & bytes);
    virtual void OnClosed(Connection * conn);

private:
    UserProxy(
        Connection::Ptr & conn
        , const UserDesc::Ptr & info
        , const std::string & box_id);

private:
    Delegate * delegate_;
    Connection::Ptr conn_;
    const UserDesc::Ptr info_;
    const std::string box_id_;
};


}  // namespace codebase


#endif  // YGG_CODEBASE_USER_PROXY_H_
