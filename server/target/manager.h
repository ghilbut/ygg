#ifndef YGG_SERVER_TARGET_MANAGER_H_
#define YGG_SERVER_TARGET_MANAGER_H_

#include "base_context.h"
#include "ctrl_ready.h"
#include "target_ready.h"
#include <string>
#include <unordered_map>


namespace ygg {
namespace server {
namespace target {


class Manager
    : public CtrlReady::Delegate
    , public TargetReady::Delegate
    , public BaseContext::Delegate {

public:
    Manager();
    ~Manager();

    void SetCtrlConnection(Connection::Ptr & conn);
    void SetTargetConnection(Connection::Ptr & conn);

    virtual void OnProxy(CtrlProxy::Ptr & ctrl);
    virtual void OnProxy(TargetProxy::Ptr & target);
    virtual void OnClosed(BaseContext * adapter);


private:
    CtrlReady ctrl_ready_;
    TargetReady target_ready_;
    std::unordered_map<std::string, BaseContext::Ptr> contexts_;
};


}  // namespace target
}  // namespace server
}  // namespace ygg


#endif  // YGG_SERVER_TARGET_MANAGER_H_
