#ifndef YGG_LIBTARGET_SERVER_TARGET_PROXY_H_
#define YGG_LIBTARGET_SERVER_TARGET_PROXY_H_

#include "core/connection.h"
#include "core/object.h"
#include "core/target_desc.h"


namespace ygg {

typedef TargetDesc TargetDesc;
typedef Connection Connection;
typedef Text Text;
typedef Bytes Bytes;

namespace target {
namespace server {


class CtrlProxy;


class TargetProxy : public Object, public Connection::Delegate {
public:
    class Delegate {
    public:
        virtual void OnText(TargetProxy * target,
                            const Text & text) = 0;
        virtual void OnBinary(TargetProxy * target,
                              const Bytes & bytes) = 0;
        virtual void OnClosed(TargetProxy * target) = 0;

    protected:
        virtual ~Delegate() {}
    };

public:
    typedef boost::intrusive_ptr<TargetProxy> Ptr;

    static Ptr New(Connection::Ptr & conn, TargetDesc::Ptr & desc);
    ~TargetProxy();

    void BindDelegate(Delegate * delegate);
    void UnbindDelegate();

    virtual void OnText(Connection * conn, const Text & text);
    virtual void OnBinary(Connection * conn, const Bytes & bytes);
    virtual void OnClosed(Connection * conn);

private:
    TargetProxy(Connection::Ptr & conn, TargetDesc::Ptr & desc);


private:
    Delegate * delegate_;
    Connection::Ptr conn_;
    TargetDesc::Ptr desc_;
};


}  // namespace server
}  // namespace target
}  // namespace ygg


#endif  // YGG_LIBTARGET_SERVER_TARGET_PROXY_H_
