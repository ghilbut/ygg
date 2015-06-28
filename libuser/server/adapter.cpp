#include "adapter.h"


namespace user {
namespace server {


Adapter::Ptr Adapter::New(UserProxy::Ptr & user, Delegate * delegate) {
    assert(user != nullptr);
    assert(delegate != nullptr);

    return new Adapter(user, delegate);
}

void Adapter::SetBox(BoxProxy::Ptr & box) {
    assert(box != nullptr);
    assert(box_ == nullptr);

    box_ = box;
    box_->BindDelegate(this);
}

void Adapter::Close() {

    if (user_ != nullptr) {
        user_->UnbindDelegate();
        user_->Close();
        user_ = nullptr;
    }

    if (box_ != nullptr) {
        box_->UnbindDelegate();
        box_->Close();
        box_ = nullptr;
    }

    delegate_->OnClosed(this);
}

void Adapter::OnText(BoxProxy * box, const std::string & text) {
    assert(box != nullptr);
    assert(box == box_);
    assert(user_ != nullptr);

    user_->SendText(text);
}

void Adapter::OnBinary(BoxProxy * box, const std::vector<uint8_t> & bytes) {
    assert(box != nullptr);
    assert(box == box_);
    assert(user_ != nullptr);

    user_->SendBinary(bytes);
}

void Adapter::OnClosed(BoxProxy * box) {
    assert(box != nullptr);
    assert(box == box_);
    assert(user_ != nullptr);

    box_->UnbindDelegate();
    box_ = nullptr;

    Close();
}

void Adapter::OnText(UserProxy * user, const std::string & text) {
    assert(user != nullptr);
    assert(user == user_);
    assert(box_ != nullptr);

    box_->SendText(text);
}

void Adapter::OnBinary(UserProxy * user, const std::vector<uint8_t> & bytes) {
    assert(user != nullptr);
    assert(user == user_);
    assert(box_ != nullptr);

    box_->SendBinary(bytes);
}

void Adapter::OnClosed(UserProxy * user) {
    assert(user != nullptr);
    assert(user == user_);
    assert(box_ != nullptr);

    user_->UnbindDelegate();
    user_ = nullptr;

    Close();
}

Adapter::Adapter(UserProxy::Ptr & user, Delegate * delegate) 
    : Object(this), user_(user), delegate_(delegate) {

    user_->BindDelegate(this);
}

Adapter::~Adapter() {
    Close();
}


}  // namespace server
}  // namesapce user
