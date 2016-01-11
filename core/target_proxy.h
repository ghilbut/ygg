#ifndef YGG_CORE_TARGET_PROXY_H_
#define YGG_CORE_TARGET_PROXY_H_

#include "connection.h"
#include "target_desc.h"
#include <string>


namespace ygg {


class TargetProxy
    : public Object
    , public Connection::Delegate {

public:
    typedef boost::intrusive_ptr<TargetProxy> Ptr;
    typedef BaseDelegate<TargetProxy> Delegate;

public:
    static Ptr New(Connection::Ptr & conn, const TargetDesc::Ptr & desc);

    virtual ~TargetProxy();

    size_t SendText(const std::string & text) const;
    size_t SendBinary(const std::vector<uint8_t> & bytes) const;
    void Close();

    void BindDelegate(Delegate * delegate);
    void UnbindDelegate();

    const TargetDesc & desc() const;

    // core::Connection::Delegate
    virtual void OnText(Connection * conn, const Text & text);
    virtual void OnBinary(Connection * conn, const Bytes & bytes);
    virtual void OnClosed(Connection * conn);

private:
    TargetProxy(Connection::Ptr & conn, const TargetDesc::Ptr & desc);


private:
    Delegate * delegate_;
    Connection::Ptr conn_;
    const TargetDesc::Ptr desc_;
};


}  // namespace ygg


#endif  // YGG_CORE_TARGET_PROXY_H_
