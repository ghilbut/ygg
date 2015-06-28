#include "box_manager.h"


namespace box {


BoxManager::BoxManager() {
    box_ready_.BindDelegate(this);
    user_ready_.BindDelegate(this);
}

void BoxManager::BindBoxConnection(Connection::Ptr & conn) {
    box_ready_.SetConnection(conn);
}

void BoxManager::BindUserConnection(Connection::Ptr & conn) {
    user_ready_.SetConnection(conn);
}

void BoxManager::OnReady(BoxProxy::Ptr & box) {
    auto rule = rule::Bypass::New(box, this);
    box_list_[box->info().id()] = rule;
}

void BoxManager::OnReady(UserProxy::Ptr & user) {

    auto itr = box_list_.find(user->box_id());
    if (itr == box_list_.end()) {
        // TODO(ghilbut): dissconnect user
        user->Close();
        return;
    }

    rule::Bypass::Ptr rule(itr->second);
    rule->SetUser(user);
}

void BoxManager::OnClosed(rule::Bypass * rule) {

}


}  // namespace box
