#ifndef YGG_CODEBASE_READY_H_
#define YGG_CODEBASE_READY_H_

#include "session_delegate.h"
#include <set>


namespace codebase {


template<class ProxtT>
class Ready : public Session::Delegate {
public:
    class Delegate {
    public:
        virtual void OnReady(ProxtT * proxy) = 0;
    protected:
        ~Delegate();
    };


public:
    Ready() {}
    virtual ~Ready() {}

    void SetSession(Session::Ptr & session) {
        readys_.insert(session);
        session->BindDelegate(this);
    }

    bool HasSession(Session::Ptr & session) const {
        return (readys_.find(session) != readys_.end());
    }

    // Session::Delegate
    virtual void OnText(Session * session, const std::string & text) {

        Session::Ptr ptr(session);
        ProxtT * proxy = ProxtT::New(text, ptr);
        if (proxy == nullptr) {
            session->Close();
            return;
        }

        delegate_->OnReady(proxy);
    }

    virtual void OnBinary(Session * session, const uint8_t bytes[], size_t size) {
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
