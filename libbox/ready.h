#ifndef YGG_LIBBOX_READY_H_
#define YGG_LIBBOX_READY_H_

#include "net/session_delegate.h"
#include <set>


using namespace net;


namespace box {


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


}  // namespace box


#endif  // YGG_LIBBOX_READY_H_
