#ifndef YGG_LIBBOX_BOX_MANAGER_H_
#define YGG_LIBBOX_BOX_MANAGER_H_

#include "box_proxy.h"
#include "user_proxy.h"
#include "rule_bypass.h"
#include "codebase/ready.h"
#include <unordered_map>
#include <string>


using namespace codebase;


namespace box {


typedef Ready<BoxProxy> BoxReady;
typedef Ready<UserProxy> UserReady;


class BoxManager
    : public BoxReady::Delegate
    , public UserReady::Delegate
    , public rule::Bypass::Delegate {

public:
    BoxManager();
    ~BoxManager() {}

    void BindBoxSession(Session::Ptr & session);
    void BindUserSession(Session::Ptr & session);

    // Ready<T>::Delegate
    virtual void OnReady(BoxProxy::Ptr & box);
    virtual void OnReady(UserProxy::Ptr & user);

    // rule::Bypass::Delegate
    virtual void OnClosed(rule::Bypass * rule);

private:
    Ready<BoxProxy> box_ready_;
    Ready<UserProxy> user_ready_;
    std::unordered_map<std::string, rule::Bypass::Ptr> box_list_;
};


}  // namespace box


#endif  // YGG_LIBBOX_BOX_MANAGER_H_
