#ifndef YGG_CODEBASE_READY_H_
#define YGG_CODEBASE_READY_H_

#include "session_delegate.h"
#include <set>


namespace codebase {


class Ready : public Session::Delegate {
public:
    class Delegate;

public:
    Ready() {}
    virtual ~Ready() {}

    void SetSession(Session * session);
    bool HasSession(Session * session) const;

    // Session::Delegate
    virtual void OnText(Session * session, const std::string & text);
    virtual void OnBinary(Session * session, const uint8_t bytes[], size_t size);
    virtual void OnClosed(Session * session);


private:
    std::set<Session*> readys_;
};


}  // namespace codebase


#endif  // YGG_CODEBASE_READY_H_
