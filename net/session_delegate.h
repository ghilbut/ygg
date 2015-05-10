#ifndef YGG_NET_SESSION_DELEGATE_H_
#define YGG_NET_SESSION_DELEGATE_H_


#include "session.h"


namespace net {


class Session::Delegate {
public:
    virtual void OnText(Session * session, const std::string & text) = 0;
    virtual void OnBinary(Session * session, const uint8_t bytes[], size_t size) = 0;
    virtual void OnClosed(Session * session) = 0;

protected:
    virtual ~Delegate() {}
};


}  // namespace net


#endif  // YGG_NET_SESSION_DELEGATE_H_
