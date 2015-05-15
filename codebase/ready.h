#ifndef YGG_CODEBASE_READY_H_
#define YGG_CODEBASE_READY_H_

#include "session_delegate.h"
#include <set>


namespace codebase {


template<class Proxy>
class Ready : public Session::Delegate {
public:
    class Delegate {
    public:
        virtual void OnReady(Proxy * proxy) = 0;
    protected:
        ~Delegate();
    };


public:
    Ready() {}
    virtual ~Ready() {}

    void SetSession(Session * session) {
        readys_.insert(session);
        session->BindDelegate(this);
    }

    bool HasSession(Session * session) const {
        return (readys_.find(session) != readys_.end());
    }

    // Session::Delegate
    virtual void OnText(Session * session, const std::string & text) {

        Proxy * proxy = Proxy::New(text, session);
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
    std::set<Session*> readys_;
    Delegate * delegate_;
};


}  // namespace codebase


#endif  // YGG_CODEBASE_READY_H_
