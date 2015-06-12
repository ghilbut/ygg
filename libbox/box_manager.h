#ifndef YGG_LIBBOX_BOX_MANAGER_H_
#define YGG_LIBBOX_BOX_MANAGER_H_

#include "box_proxy.h"
#include "user_proxy.h"
#include "codebase/ready.h"
#include <map>
#include <set>
#include <string>


using namespace codebase;


namespace box {


typedef Ready<BoxProxy> BoxReady;
typedef Ready<UserProxy> UserReady;


class BoxManager : 
    public BoxReady::Delegate
    , public UserReady::Delegate {

public:
    BoxManager();
    ~BoxManager() {}

    void BindBoxSession(Session::Ptr & session);
    void BindUserSession(Session::Ptr & session);

    // Ready<T>::Delegate
    virtual void OnReady(BoxProxy::Ptr & box);
    virtual void OnReady(UserProxy::Ptr & user);


private:
    Ready<BoxProxy> box_ready_;
    Ready<UserProxy> user_ready_;
    std::map<std::string, BoxProxy::Ptr> box_list_;
};


}  // namespace box


#endif  // YGG_LIBBOX_BOX_MANAGER_H_
