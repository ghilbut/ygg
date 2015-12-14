#ifndef YGG_LIBUSER_SERVER_USER_MANAGER_H_
#define YGG_LIBUSER_SERVER_USER_MANAGER_H_

#include "connector.h"
#include "codebase/box_proxy.h"
#include "codebase/user_proxy.h"


using namespace codebase;


namespace user {
namespace server {


typedef Ready<BoxProxy> BoxReady;
typedef Ready<UserProxy> UserReady;


class UserManager
    : public BoxReady::Delegate 
    , public UserReady::Delegate
    , public Connector::Delegate {

public:
    UserManager(Connector & connector);
    ~UserManager();

    void BindUserConnection(Connection::Ptr & conn);

    // Ready<T>::Delegate
    virtual void OnReady(BoxProxy::Ptr & box);
    virtual void OnReady(UserProxy::Ptr & user);

    // Connector::Delegate
    virtual void OnConnected(const std::string & user, Connection::Ptr conn);

priavte:
    BoxReady box_ready_;
    UserReady user_ready_;
    Connector & connector_;
};


}  // namespace server
}  // namespace user


#endif  // YGG_LIBUSER_SERVER_USER_MANAGER_H_
