#ifndef YGG_CORE_PROXY_H_
#define YGG_CORE_PROXY_H_

#include "connection.h"


namespace ygg {


template<class DescT>
class Proxy
    : public Object
    , public Connection::Delegate {

public:
    typedef boost::intrusive_ptr<Proxy<DescT>> Ptr;
    typedef BaseDelegate<Proxy<DescT>> Delegate;

public:
    static typename Proxy::Ptr
    New(Connection::Ptr & conn, const typename DescT::Ptr & desc) {
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

    inline const DescT & desc() const {
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
    Proxy(Connection::Ptr & conn, const typename DescT::Ptr &  desc)
        : Object()
        , delegate_(nullptr)
        , conn_(conn)
        , desc_(desc) {

            assert(conn_ != nullptr);
            assert(desc_ != nullptr);
            conn_->BindDelegate(this);
        }


private:
    class NullDelegate : public Delegate {
    public:
        virtual void OnText(Proxy*, const Text&) {}
        virtual void OnBinary(Proxy*, const Bytes&) {}
        virtual void OnClosed(Proxy*) {}
    };

    static NullDelegate kNullDelegate;

    Delegate * delegate_;
    Connection::Ptr conn_;
    const typename DescT::Ptr desc_;
};


}  // namespace ygg


#endif  // YGG_CORE_PROXY_H_
