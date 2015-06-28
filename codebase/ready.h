#ifndef YGG_CODEBASE_READY_H_
#define YGG_CODEBASE_READY_H_

#include "connection.h"
#include <set>


namespace codebase {


template<class ProxyT>
class Ready : public Connection::Delegate {
public:
    class Delegate {
    public:
        virtual void OnReady(typename ProxyT::Ptr & proxy) = 0;
    protected:
        virtual ~Delegate() {}
    };


public:
    Ready() : delegate_(nullptr) {}
    virtual ~Ready() {}

    void BindDelegate(Delegate * delegate) {
        delegate_ = delegate;
    }

    void UnbindDelegate() {
        delegate_ = nullptr;
    }

    void SetConnection(Connection::Ptr & conn) {
        readys_.insert(conn);
        conn->BindDelegate(this);
    }

    bool HasConnection(Connection::Ptr & conn) const {
        return (readys_.find(conn) != readys_.end());
    }

    virtual typename ProxyT::Ptr NewProxy(Connection * conn, const std::string & text) const {
        Connection::Ptr ptr(conn);
        return ProxyT::New(ptr, text);
    }

    // Connection::Delegate
    virtual void OnText(Connection * conn, const std::string & text) {

        if (readys_.find(conn) == readys_.end()) {
            conn->Close();
            return;
        }

        readys_.erase(conn);

        if (delegate_ == nullptr) {
            conn->Close();
        }

        typename ProxyT::Ptr proxy(NewProxy(conn, text));
        if (proxy == nullptr) {
            conn->Close();
            return;
        }

        delegate_->OnReady(proxy);
    }

    virtual void OnBinary(Connection * conn, const std::vector<uint8_t> & bytes) {
        // nothing
    }

    virtual void OnClosed(Connection * conn) {
        readys_.erase(conn);
    }


private:
    std::set<Connection::Ptr> readys_;
    Delegate * delegate_;
};


}  // namespace codebase


#endif  // YGG_CODEBASE_READY_H_
