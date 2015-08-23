#ifndef YGG_CORE_PROXY_H_
#define YGG_CORE_PROXY_H_

#include "codebase/connection.h"


namespace core {


template<class Desc>
class Proxy
    : public codebase::Object<Proxy<Desc>>
    , public Connection::Delegate {

public:
    class Delegate {
    public:
        virtual void OnText(Proxy * user, const std::string & text) = 0;
        virtual void OnBinary(Proxy * user, const std::vector<uint8_t> & bytes) = 0;
        virtual void OnClosed(Proxy * user) = 0;
    };

public:
    static typename Proxy::Ptr
    New(Connection::Ptr & conn, const typename Desc::Ptr & desc) {
        assert(conn != nullptr);
        assert(desc != nullptr);
        return new Proxy(conn, desc);
    }

    virtual ~Proxy() {
        Close();
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
    virtual void OnText(Connection * conn, const std::string & text) {
        assert(conn != nullptr);
        assert(conn == conn_);

        if (delegate_ != nullptr) {
            delegate_->OnText(this, text);
        }
    }

    virtual void OnBinary(Connection * conn, const std::vector<uint8_t> & bytes) {
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
        : codebase::Object<Proxy<Desc>>(this)
        , delegate_(nullptr)
        , conn_(conn)
        , desc_(desc) {

            assert(conn_ != nullptr);
            assert(desc_ != nullptr);
            conn_->BindDelegate(this);
        }


private:
    Delegate * delegate_;
    Connection::Ptr conn_;
    const typename Desc::Ptr desc_;
};


}  // namespace core


#endif  // YGG_CORE_PROXY_H_
