#include "manager.h"


namespace ygg {
namespace target {
namespace server {


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

    auto itr = adapters_.find(endpoint);
    if (itr != adapters_.end()) {
        auto & adapter = itr->second;
        adapter->SetCtrl(ctrl);
    } else {
        // TODO(ghilbut): log error and notify
    }
}

void Manager::OnProxy(TargetProxy::Ptr & target) {

    const auto & desc = target->desc();
    const auto & endpoint = desc.endpoint;

    auto itr = adapters_.find(endpoint);
    if (itr == adapters_.end()) {
        auto adapter(BypassAdapter::New(target, this));
        adapters_[endpoint] = adapter;
    } else {
        // TODO(ghilbut): log error and notify
        //                close adapter already exists
    }
}

void Manager::OnClosed(BypassAdapter * adapter) {
    assert(adapter != nullptr);

    const auto & target = adapter->target();
    const auto & desc = target->desc();
    const auto & endpoint = desc.endpoint;

    if (adapters_.find(endpoint) != adapters_.end()) {
        adapters_.erase(endpoint);
    } else {
        // TODO(ghilbut): log and notify warning
    }
}


}  // namespace ygg
}  // namespace target
}  // namespace server
