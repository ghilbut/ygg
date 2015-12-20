#ifndef YGG_LIBTARGET_SERVER_CTRL_READY_H_
#define YGG_LIBTARGET_SERVER_CTRL_READY_H_

#include "codebase/object.h"
#include "core/proxy.h"
#include "core/ctrl_desc.h"
#include "net/connection.h"
#include "net/delegate.h"
#include <unordered_set>

using namespace ygg::net;


namespace ygg {
namespace target {
namespace server {

typedef Proxy<CtrlDesc> CtrlProxy;


class CtrlReady : public Delegate<Connection> {
public: 
    class Delegate {
    public:
        virtual void OnProxy(CtrlProxy::Ptr & proxy) = 0;

    protected:
        virtual ~Delegate() {}
    };

public:
    CtrlReady(Delegate * delegate);
    ~CtrlReady() {}

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


#endif  // YGG_LIBTARGET_SERVER_CTRL_READY_H_
