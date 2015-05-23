#include "box_server.h"


namespace box {


BoxServer::BoxServer() {
    box_ready_.BindDelegate(this);
    user_ready_.BindDelegate(this);
}

void BoxServer::BindBoxSession(Session::Ptr & session) {
    box_ready_.SetSession(session);
}

void BoxServer::BindUserSession(Session::Ptr & session) {
    user_ready_.SetSession(session);
}

void BoxServer::OnReady(BoxProxy::Ptr box) {
    box_list_[box->info().id()] = box;
}

void BoxServer::OnReady(UserProxy::Ptr user) {

    auto itr = box_list_.find(user->box_id());
    if (itr == box_list_.end()) {
        // TODO(ghilbut): dissconnect user
        return;
    }

    // TODO(ghilbut): bind to box
}


}  // namespace box
