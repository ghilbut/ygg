#ifndef YGG_LIBBOX_USER_READY_H_
#define YGG_LIBBOX_USER_READY_H_

#include "net/session_delegate.h"
#include <set>


using namespace net;


namespace box {


class UserReady : public Session::Delegate {
public:
    UserReady() {}
    ~UserReady() {}

    void SetUserSession(Session * session);
    bool HasUserSession(Session * session) const;

    // Session::Delegate
    virtual void OnText(Session * session, const std::string & text);
    virtual void OnBinary(Session * session, const uint8_t bytes[], size_t size);
    virtual void OnClosed(Session * session);


private:
    std::set<Session*> readys_;
};


}  // namespace box


#endif  // YGG_LIBBOX_USER_READY_H_
