#include "box_proxy.h"

#include "box_info.h"
#include <cassert>


using namespace codebase;


namespace box {


BoxProxy::Ptr BoxProxy::New(Session::Ptr & session, const std::string & json) {

    assert(session != nullptr);
    assert(!json.empty());

    BoxInfo * info = BoxInfo::New(json);
    if (info == nullptr) {
        return nullptr;
    }

    return new BoxProxy(session, info);
}

BoxProxy::~BoxProxy() {
}

const BoxInfo * BoxProxy::info() const {
    return info_;
}

void BoxProxy::OnText(Session * session, const std::string & text) {
}

void BoxProxy::OnBinary(Session * session, const uint8_t bytes[], size_t size) {
}

void BoxProxy::OnClosed(Session * session) {
}

BoxProxy::BoxProxy(Session::Ptr & session, const BoxInfo * info)
    : Object(), session_(session), info_(info) {

}


}  // namespace box
