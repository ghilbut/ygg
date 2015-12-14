#ifndef YGG_CORE_PROXY_H_
#define YGG_CORE_PROXY_H_

#include "net/connection.h"


using namespace ygg::net;


namespace ygg {
namespace core {


template<class Desc>
class Proxy
    : public Object
    , public Connection::Delegate {

public:
    typedef boost::intrusive_ptr<Proxy<Desc>> Ptr;
    typedef net::Delegate<Proxy<Desc>> Delegate;

public:
    static typename Proxy::Ptr
    New(Connection::Ptr & conn, const typename Desc::Ptr & desc) {
        assert(conn != nullptr);
        assert(desc != nullptr);
        return new Proxy(conn, desc);
    }

    virtual ~Proxy() {
        // NOTE(ghilbut): below code makes 
        // "libc++abi.dylib: Pure virtual function called!" runtime error
        // because calling Close() function of Connection interface
        // Close();
    }

    inline size_t SendText(const std::string & text) const {
        return (conn_ != nullptr ? conn_->SendText(text) : 0);
    }

    inline size_t SendBinary(const std::vector<uint8_t> & bytes) const {
        return (conn_ != nullptr ? conn_->SendBinary(bytes) : 0);
    }

    inline void Close() {
        if (conn_ != nullptr) {
            conn_->Close();
        }
    }

    inline void BindDelegate(Delegate * delegate) {
        assert(delegate != nullptr);
        delegate_ = delegate;
    }

    inline void UnbindDelegate() {
        delegate_ = nullptr;
    }

    inline const Desc & desc() const {
        return *desc_;
    }

    // core::Connection::Delegate
    virtual void OnText(Connection * conn, const Text & text) {
        assert(conn != nullptr);
        assert(conn == conn_);

        if (delegate_ != nullptr) {
            delegate_->OnText(this, text);
        }
    }

    virtual void OnBinary(Connection * conn, const Bytes & bytes) {
        assert(conn != nullptr);
        assert(conn == conn_);
        
        if (delegate_ != nullptr) {
            delegate_->OnBinary(this, bytes);
        }
    }

    virtual void OnClosed(Connection * conn) {
        assert(conn != nullptr);
        assert(conn == conn_);

        conn_->UnbindDelegate();
        conn_ = nullptr;

        if (delegate_ != nullptr) {
            delegate_->OnClosed(this);
        }
    }

private:
    Proxy(Connection::Ptr & conn, const typename Desc::Ptr &  desc)
        : Object()
        , delegate_(nullptr)
        , conn_(conn)
        , desc_(desc) {

            assert(conn_ != nullptr);
            assert(desc_ != nullptr);
            conn_->BindDelegate(this);
        }


private:
    typedef net::NullDelegate<Proxy<Desc>> NullDelegate;
    static NullDelegate kNullDelegate;

    Delegate * delegate_;
    Connection::Ptr conn_;
    const typename Desc::Ptr desc_;
};

//Proxy::NullDelegate kNullDelegate;


}  // namespace core
}  // namespace ygg


#endif  // YGG_CORE_PROXY_H_
