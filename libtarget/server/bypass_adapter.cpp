#include "bypass_adapter.h"



namespace ygg {
namespace target {
namespace server {


BypassAdapter::Ptr BypassAdapter::New(TargetProxy::Ptr & target) {
    return new BypassAdapter(target);
}

BypassAdapter::BypassAdapter(TargetProxy::Ptr & target)
    : target_(target) {

    assert(target != nullptr);

    target_->BindDelegate(this);
}

BypassAdapter::~BypassAdapter() {
    // nothing
}

void BypassAdapter::SetCtrl(CtrlProxy::Ptr & ctrl) {
    assert(ctrls_.find(ctrl) == ctrls_.end());

    ctrls_.insert(ctrl);
    ctrl->BindDelegate(this);
}

bool BypassAdapter::HasCtrl(CtrlProxy::Ptr & ctrl) const {
    assert(ctrl != nullptr);
    return (ctrls_.find(ctrl) != ctrls_.end());
}
    
void BypassAdapter::OnText(CtrlProxy * ctrl, const Text & text) {
    assert(ctrl != nullptr);
    assert(ctrls_.find(ctrl) != ctrls_.end());

    target_->SendText(text);
}

void BypassAdapter::OnBinary(CtrlProxy * ctrl, const Bytes & bytes) {
    assert(ctrl != nullptr);
    assert(ctrls_.find(ctrl) != ctrls_.end());

    target_->SendBinary(bytes);
}

void BypassAdapter::OnClosed(CtrlProxy * ctrl) {
    assert(ctrl != nullptr);
    assert(ctrls_.find(ctrl) != ctrls_.end());

    target_->Close();
}

void BypassAdapter::OnText(TargetProxy * target, const Text & text) {
    assert(target != nullptr);
    assert(target == target_);

    for (auto ctrl : ctrls_) {
        ctrl->SendText(text);
    }
}

void BypassAdapter::OnBinary(TargetProxy * target, const Bytes & bytes) {
    assert(target != nullptr);
    assert(target == target_);

    for (auto ctrl : ctrls_) {
        ctrl->SendBinary(bytes);
    }
}

void BypassAdapter::OnClosed(TargetProxy * target) {
    assert(target != nullptr);
    assert(target == target_);

    for (auto ctrl : ctrls_) {
        ctrl->Close();
    }
}


}  // namespace server
}  // namespace target
}  // namespace ygg

