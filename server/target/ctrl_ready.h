#ifndef YGG_SERVER_TARGET_CTRL_READY_H_
#define YGG_SERVER_TARGET_CTRL_READY_H_

#include "core/connection.h"
#include "core/ctrl_desc.h"
#include "core/object.h"
#include "core/proxy.h"
#include <unordered_set>


namespace ygg {
namespace server {
namespace target {


typedef Proxy<CtrlDesc> CtrlProxy;


class CtrlReady : public Connection::Delegate {
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


}  // namespace target
}  // namespace server
}  // namespace ygg


#endif  // YGG_SERVER_TARGET_CTRL_READY_H_
