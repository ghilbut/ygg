#include "base_context.h"



namespace ygg {
namespace server {
namespace target {


BaseContext::Ptr BaseContext::New(TargetProxy::Ptr & target, 
                                      Delegate * delegate) {
    return new BaseContext(target, delegate);
}

BaseContext::BaseContext(TargetProxy::Ptr & target, 
                             Delegate * delegate)
    : target_(target), delegate_(delegate) {

    assert(target != nullptr);
    assert(delegate != nullptr);

    target_->BindDelegate(this);
}

BaseContext::~BaseContext() {
    // nothing
}

void BaseContext::SetCtrl(CtrlProxy::Ptr & ctrl) {
    assert(ctrls_.find(ctrl) == ctrls_.end());

    ctrls_.insert(ctrl);
    ctrl->BindDelegate(this);
}

bool BaseContext::HasCtrl(CtrlProxy::Ptr & ctrl) const {
    assert(ctrl != nullptr);
    return (ctrls_.find(ctrl) != ctrls_.end());
}

TargetProxy::Ptr & BaseContext::target() {
    return target_;
}
    
void BaseContext::OnText(CtrlProxy * ctrl, const Text & text) {
    assert(ctrl != nullptr);
    assert(ctrls_.find(ctrl) != ctrls_.end());

    target_->SendText(text);
}

void BaseContext::OnBinary(CtrlProxy * ctrl, const Bytes & bytes) {
    assert(ctrl != nullptr);
    assert(ctrls_.find(ctrl) != ctrls_.end());

    target_->SendBinary(bytes);
}

void BaseContext::OnClosed(CtrlProxy * ctrl) {
    assert(ctrl != nullptr);
    assert(ctrls_.find(ctrl) != ctrls_.end());

    ctrls_.erase(ctrl);
}

void BaseContext::OnText(TargetProxy * target, const Text & text) {
    assert(target != nullptr);
    assert(target == target_);

    for (auto ctrl : ctrls_) {
        ctrl->SendText(text);
    }
}

void BaseContext::OnBinary(TargetProxy * target, const Bytes & bytes) {
    assert(target != nullptr);
    assert(target == target_);

    for (auto ctrl : ctrls_) {
        ctrl->SendBinary(bytes);
    }
}

void BaseContext::OnClosed(TargetProxy * target) {
    assert(target != nullptr);
    assert(target == target_);

    target->UnbindDelegate();
    for (auto ctrl : ctrls_) {
        ctrl->UnbindDelegate();
        ctrl->Close();
    }

    delegate_->OnClosed(this);
}


}  // namespace target
}  // namespace server
}  // namespace ygg

