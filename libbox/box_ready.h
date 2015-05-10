#ifndef YGG_LIBBOX_BOX_READY_H_
#define YGG_LIBBOX_BOX_READY_H_

#include "net/session_delegate.h"
#include <set>


using namespace net;


namespace box {


class BoxReady : public Session::Delegate {
public:
    BoxReady() {}
    ~BoxReady() {}

    void SetBoxSession(Session * session);
    bool HasBoxSession(Session * session) const;

    // WebSocket::Delegate
    virtual void OnText(Session * session, const std::string & text);
    virtual void OnBinary(Session * session, const uint8_t bytes[], size_t size);
    virtual void OnClosed(Session * session);


private:
    std::set<Session*> readys_;
};


}  // namespace box


#endif  // YGG_LIBBOX_BOX_READY_H_
