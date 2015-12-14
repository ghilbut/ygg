#ifndef YGG_NET_CONNECTION_H_
#define YGG_NET_CONNECTION_H_

#include "delegate.h"
#include "codebase/object.h"


namespace ygg {
namespace net {


class Connection : public Object {
public:
    typedef boost::intrusive_ptr<Connection> Ptr;
    typedef Delegate<Connection> Delegate;

    virtual size_t SendText(const Text & text) const = 0;
    virtual size_t SendBinary(const Bytes & bytes) const = 0;
    virtual void Close() = 0;

    void FireOnTextEvent(const Text & text);
    void FireOnBinaryEvent(const Bytes & bytes);
    void FireOnClosedEvent();

    void BindDelegate(Delegate * delegate);
    void UnbindDelegate();

protected:
    static NullDelegate<Connection> kNullDelegate;
    explicit Connection(Delegate * delegate = &kNullDelegate);
    virtual ~Connection() {}

private:
    Delegate * delegate_;
};


}  // namespace net
}  // namespace ygg


#endif  // YGG_NET_CONNECTION_H_
