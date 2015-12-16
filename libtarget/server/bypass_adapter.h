#ifndef YGG_LIBTARGET_SERVER_BYPASS_ADAPTER_H_
#define YGG_LIBTARGET_SERVER_BYPASS_ADAPTER_H_

#include "core/ctrl_desc.h"
#include "core/target_desc.h"
#include "core/proxy.h"
#include <boost/intrusive_ptr.hpp>
#include <unordered_set>


namespace ygg {


typedef core::CtrlDesc CtrlDesc;
typedef core::TargetDesc TargetDesc;
typedef core::Proxy<CtrlDesc> CtrlProxy;
typedef core::Proxy<TargetDesc> TargetProxy;


namespace target {
namespace server {


class BypassAdapter
    : public Object
    , public CtrlProxy::Delegate
    , public TargetProxy::Delegate {

public:
    typedef boost::intrusive_ptr<BypassAdapter> Ptr;
    
    static Ptr New(TargetProxy::Ptr & target);

    void SetCtrl(CtrlProxy::Ptr & ctrl);
    bool HasCtrl(CtrlProxy::Ptr & ctrl) const;
    
    virtual void OnText(CtrlProxy * target, const Text & text);
    virtual void OnBinary(CtrlProxy * target, const Bytes & bytes);
    virtual void OnClosed(CtrlProxy * target);

    virtual void OnText(TargetProxy * target, const Text & text);
    virtual void OnBinary(TargetProxy * target, const Bytes & bytes);
    virtual void OnClosed(TargetProxy * target);

private:
    BypassAdapter(TargetProxy::Ptr & target);
    ~BypassAdapter();


private:
    std::unordered_set<CtrlProxy::Ptr> ctrls_;
    TargetProxy::Ptr target_;
};


}  // namespace server
}  // namespace target
}  // namespace ygg


#endif  // YGG_LIBTARGET_SERVER_BYPASS_ADAPTER_H_
