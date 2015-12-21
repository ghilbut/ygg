#ifndef YGG_SERVER_TARGET_BASE_CONTEXT_H_
#define YGG_SERVER_TARGET_BASE_CONTEXT_H_

#include "core/ctrl_desc.h"
#include "core/target_desc.h"
#include "core/proxy.h"
#include <boost/intrusive_ptr.hpp>
#include <unordered_set>


namespace ygg {


typedef Proxy<CtrlDesc> CtrlProxy;
typedef Proxy<TargetDesc> TargetProxy;


namespace server {
namespace target {


class BaseContext
    : public Object
    , public CtrlProxy::Delegate
    , public TargetProxy::Delegate {

public:
    class Delegate {
    public:
        virtual void OnClosed(BaseContext * adapter) = 0;

    protected:
        virtual ~Delegate() {}
    };

public:
    typedef boost::intrusive_ptr<BaseContext> Ptr;
    
    static Ptr New(TargetProxy::Ptr & target, Delegate * delegate);

    void SetCtrl(CtrlProxy::Ptr & ctrl);
    bool HasCtrl(CtrlProxy::Ptr & ctrl) const;

    TargetProxy::Ptr & target();
    
    virtual void OnText(CtrlProxy * target, const Text & text);
    virtual void OnBinary(CtrlProxy * target, const Bytes & bytes);
    virtual void OnClosed(CtrlProxy * target);

    virtual void OnText(TargetProxy * target, const Text & text);
    virtual void OnBinary(TargetProxy * target, const Bytes & bytes);
    virtual void OnClosed(TargetProxy * target);

protected:
    BaseContext(TargetProxy::Ptr & target, Delegate * delegate);
    virtual ~BaseContext();


private:
    Delegate * delegate_;

    std::unordered_set<CtrlProxy::Ptr> ctrls_;
    TargetProxy::Ptr target_;
};


}  // namespace target
}  // namespace server
}  // namespace ygg


#endif  // YGG_SERVER_TARGET_BASE_CONTEXT_H_
