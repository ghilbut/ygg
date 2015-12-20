#ifndef YGG_LIBTARGET_SERVER_TARGET_READY_H_
#define YGG_LIBTARGET_SERVER_TARGET_READY_H_

#include "core/connection.h"
#include "core/delegate.h"
#include "core/object.h"
#include "core/proxy.h"
#include "core/target_desc.h"
#include <unordered_set>


namespace ygg {
namespace target {
namespace server {

typedef Proxy<TargetDesc> TargetProxy;


class TargetReady : public Delegate<Connection> {
public: 
    class Delegate {
    public:
        virtual void OnProxy(TargetProxy::Ptr & proxy) = 0;

    protected:
        virtual ~Delegate() {}
    };

public:
    TargetReady(Delegate * delegate);
    ~TargetReady() {}

    void SetConnection(Connection::Ptr conn);
    bool HasConnection(Connection::Ptr conn);

    virtual void OnText(Connection * conn, const Text & text);
    virtual void OnBinary(Connection * conn, const Bytes & bytes);
    virtual void OnClosed(Connection * conn);


private:
    Delegate * delegate_;
    std::unordered_set<Connection::Ptr> conn_set_;
};


}  // namespace server
}  // namespace target
}  // namespace ygg


#endif  // YGG_LIBTARGET_SERVER_TARGET_READY_H_
