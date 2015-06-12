#include "rule_bypass.h"


namespace box {
namespace rule {


Bypass::Bypass(BoxProxy::Ptr & box, Delegate * delegate)
    : box_(box), delegate_(delegate) {

    assert(box_ != nullptr);
    box_->BindDelegate(this);
}

Bypass::~Bypass() {
    Close();    
}

void Bypass::SetUser(UserProxy::Ptr & user) {
    assert(user_list_.find(user) == user_list_.end());
    user_list_.insert(user);
    user->BindDelegate(this);
}

void Bypass::Close() {
    assert(box_ != nullptr);

    box_->UnbindDelegate();
    for (auto user : user_list_) {
        user->UnbindDelegate();
        user->Close();
    }
    user_list_.clear();
}

void Bypass::OnText(BoxProxy * box, const std::string & text) {
    assert(box != nullptr);
    assert(box == box_);
    printf("Bypass::OnText\n");
    for (auto user : user_list_) {
        user->SendText(text);
    }
}

void Bypass::OnBinary(BoxProxy * box, const std::vector<uint8_t> & bytes) {
    assert(box != nullptr);
    assert(box == box_);
    for (auto user : user_list_) {
        user->SendBinary(bytes);
    }
}

void Bypass::OnClosed(BoxProxy * box) {
    assert(box != nullptr);
    assert(box == box_);
    for (auto user : user_list_) {
        user->UnbindDelegate();
        user->Close();
    }
}

void Bypass::OnText(UserProxy * user, const std::string & text) {
    assert(user != nullptr);
    assert(user_list_.find(user) != user_list_.end());
    assert(box_ != nullptr);
    box_->SendText(text);
}

void Bypass::OnBinary(UserProxy * user, const std::vector<uint8_t> & bytes) {
    assert(user != nullptr);
    assert(user_list_.find(user) != user_list_.end());
    assert(box_ != nullptr);
    box_->SendBinary(bytes);
}

void Bypass::OnClosed(UserProxy * user) {
    assert(user != nullptr);
    assert(user_list_.find(user) != user_list_.end());
    user_list_.erase(user);
}


}  // namespace rule
}  // namepsace box
