#ifndef YGG_LIBTARGET_SERVER_MANAGER_H_
#define YGG_LIBTARGET_SERVER_MANAGER_H_

#include "bypass_adapter.h"
#include "ctrl_ready.h"
#include "target_ready.h"
#include <string>
#include <unordered_map>


namespace ygg {
namespace target {
namespace server {


class Manager
    : public CtrlReady::Delegate
    , public TargetReady::Delegate
    , public BypassAdapter::Delegate {

public:
    Manager();
    ~Manager();

    void SetCtrlConnection(Connection::Ptr & conn);
    void SetTargetConnection(Connection::Ptr & conn);

    virtual void OnProxy(CtrlProxy::Ptr & ctrl);
    virtual void OnProxy(TargetProxy::Ptr & target);
    virtual void OnClosed(BypassAdapter * adapter);


private:
    CtrlReady ctrl_ready_;
    TargetReady target_ready_;
    std::unordered_map<std::string, BypassAdapter::Ptr> adapters_;
};


}  // namespace server
}  // namespace target
}  // namespace ygg


#endif  // YGG_LIBTARGET_SERVER_MANAGER_H_
