#ifndef YGG_LIBBOX_BOX_READY_H_
#define YGG_LIBBOX_BOX_READY_H_

#include "net/http_websocket_delegate.h"
#include <set>


using namespace net::http;


namespace box {


class BoxReady : public WebSocket::Delegate {
public:
    BoxReady() {}
    ~BoxReady() {}

    void SetBoxConnection(WebSocket * ws);
    bool HasBoxConnection(WebSocket * ws) const;

    // WebSocket::Delegate
    virtual void OnTextMessage(WebSocket * ws, const std::string & text);
    virtual void OnClosed(WebSocket * ws);


private:
    std::set<WebSocket*> readys_;
};


}  // namespace box


#endif  // YGG_LIBBOX_BOX_READY_H_
