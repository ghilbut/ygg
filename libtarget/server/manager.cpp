#include "manaher.h"


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

void Manager::OnProxy(CtrlProxy::Ptr & proxy) {
}

void Manager::OnProxy(TargetProxy::Ptr & proxy) {
}


}  // namespace ygg
}  // namespace target
}  // namespace server
