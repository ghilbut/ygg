#ifndef YGG_CODEBASE_READY_H_
#define YGG_CODEBASE_READY_H_

#include "session_delegate.h"
#include <set>


namespace codebase {


template<class ProxyT>
class Ready : public Session::Delegate {
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

    void SetSession(Session::Ptr & session) {
        readys_.insert(session);
        session->BindDelegate(this);
    }

    bool HasSession(Session::Ptr & session) const {
        return (readys_.find(session) != readys_.end());
    }

    virtual typename ProxyT::Ptr NewProxy(Session * session, const std::string & text) const {
        Session::Ptr ptr(session);
        return ProxyT::New(ptr, text);
    }

    // Session::Delegate
    virtual void OnText(Session * session, const std::string & text) {

        if (readys_.find(session) == readys_.end()) {
            session->Close();
            return;
        }

        readys_.erase(session);

        if (delegate_ == nullptr) {
            session->Close();
        }

        typename ProxyT::Ptr proxy(NewProxy(session, text));
        if (proxy == nullptr) {
            session->Close();
        }

        delegate_->OnReady(proxy);
    }

    virtual void OnBinary(Session * session, const std::vector<uint8_t> & bytes) {
        // nothing
    }

    virtual void OnClosed(Session * session) {
        readys_.erase(session);
    }


private:
    std::set<Session::Ptr> readys_;
    Delegate * delegate_;
};


}  // namespace codebase


#endif  // YGG_CODEBASE_READY_H_
