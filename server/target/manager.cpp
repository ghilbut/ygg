#include "manager.h"


namespace ygg {
namespace server {
namespace target {


Manager::Manager()
    : ctrl_ready_(this), target_ready_(this) {
    // nothing
}

Manager::~Manager() {

}

void Manager::SetCtrlConnection(Connection::Ptr & conn) {
    assert(conn != nullptr);
    ctrl_ready_.SetConnection(conn);
}

void Manager::SetTargetConnection(Connection::Ptr & conn) {
    assert(conn != nullptr);
    target_ready_.SetConnection(conn);
}

void Manager::OnProxy(CtrlProxy::Ptr & ctrl) {

    const auto & desc = ctrl->desc();
    const auto & endpoint = desc.endpoint;

    auto itr = contexts_.find(endpoint);
    if (itr != contexts_.end()) {
        auto & context = itr->second;
        context->SetCtrl(ctrl);
    } else {
        // TODO(ghilbut): log error and notify
    }
}

void Manager::OnProxy(TargetProxy::Ptr & target) {

    const auto & desc = target->desc();
    const auto & endpoint = desc.endpoint;

    auto itr = contexts_.find(endpoint);
    if (itr == contexts_.end()) {
        auto context(BaseContext::New(target, this));
        contexts_[endpoint] = context;
    } else {
        // TODO(ghilbut): log error and notify
        //                close context already exists
    }
}

void Manager::OnClosed(BaseContext * context) {
    assert(context != nullptr);

    const auto & target = context->target();
    const auto & desc = target->desc();
    const auto & endpoint = desc.endpoint;

    if (contexts_.find(endpoint) != contexts_.end()) {
        contexts_.erase(endpoint);
    } else {
        // TODO(ghilbut): log and notify warning
    }
}


}  // namespace target
}  // namespace server
}  // namespace ygg
