#include "box_proxy.h"

#include <cassert>


using namespace codebase;


namespace box {


class NullDelegate : public BoxProxy::Delegate {
public:
    virtual void OnText(BoxProxy*, const std::string&) {}
    virtual void OnBinary(BoxProxy*, const std::vector<uint8_t>&) {}
    virtual void OnClosed(BoxProxy*) {}
};

static NullDelegate kNullDelegate;


BoxProxy::Ptr BoxProxy::New(Session::Ptr & session, const std::string & json) {

    assert(session != nullptr);
    assert(!json.empty());

    BoxInfo::Ptr info(BoxInfo::New(json));
    if (info == nullptr) {
        return nullptr;
    }

    return new BoxProxy(session, info);
}

BoxProxy::~BoxProxy() {
}

void BoxProxy::BindDelegate(Delegate * delegate) {
    assert(delegate != nullptr);
    delegate_ = delegate;
}

void BoxProxy::UnbindDelegate() {
    delegate_ = &kNullDelegate;
}

const BoxInfo & BoxProxy::info() const {
    return *info_;
}

void BoxProxy::OnText(Session * session, const std::string & text) {
    assert(session == session_);
    delegate_->OnText(this, text);
}

void BoxProxy::OnBinary(Session * session, const std::vector<uint8_t> & bytes) {
    assert(session == session_);
    delegate_->OnBinary(this, bytes);
}

void BoxProxy::OnClosed(Session * session) {
    assert(session == session_);
    delegate_->OnClosed(this);
}

BoxProxy::BoxProxy(Session::Ptr & session, const BoxInfo::Ptr & info)
    : Object()
    , delegate_(&kNullDelegate)
    , session_(session)
    , info_(info) {

    assert(session_ != nullptr);
    assert(info_ != nullptr);

    session_->BindDelegate(this);
}


}  // namespace box
