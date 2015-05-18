#include "box_proxy.h"

#include "box_info.h"
#include <cassert>


namespace box {


BoxProxy * BoxProxy::New(Session * session, const std::string & json) {
    assert(!json.empty());
    assert(session != nullptr);

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

BoxProxy::BoxProxy(Session * session, const BoxInfo * info)
    : session_(session), info_(info) {

}


}  // namespace box
