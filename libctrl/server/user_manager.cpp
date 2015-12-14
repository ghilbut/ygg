#include "user_manager.h"


namespace user {
namespace server {


UserManager::UserManager(Connector & connector) 
    : connector_(connector) {

    connector_.BindDelegate(this);
}

UserManager::~UserManager() {

    connector_.UnbindDelegate();
}

void UserManager::BindUserConnection(Connection::Ptr & conn) {
    user_ready_.SetConnection(conn);
}

void UserManager::OnReady(BoxProxy::Ptr & box) {
}

void UserManager::OnReady(UserProxy::Ptr & user) {

    const std::string user_id(user->info().id());
    const std::string endpoint(user->box_id());
    connector_.Connect(user_id, endpoint);
}

void UserManager::OnConnected(const std::string & user, Connection::Ptr conn) {
    box_ready_.SetConnection(conn);
}

void UserManager::OnError(const std::string & user) {
}


}  // namespace server
}  // namespace user
