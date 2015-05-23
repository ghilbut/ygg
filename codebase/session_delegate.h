#ifndef YGG_CODEBASE_SESSION_DELEGATE_H_
#define YGG_CODEBASE_SESSION_DELEGATE_H_


#include "session.h"


namespace codebase {


class Session::Delegate {
public:
    virtual void OnText(Session * session, const std::string & text) = 0;
    virtual void OnBinary(Session * session, const std::vector<uint8_t> & bytes) = 0;
    virtual void OnClosed(Session * session) = 0;

protected:
    virtual ~Delegate() {}
};


}  // namespace codebase


#endif  // YGG_CODEBASE_SESSION_DELEGATE_H_
