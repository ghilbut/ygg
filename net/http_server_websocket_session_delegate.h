#ifndef YGG_NET_HTTP_WEBSOCKET_DELEGATE_H_
#define YGG_NET_HTTP_WEBSOCKET_DELEGATE_H_

#include "http_websocket.h"


namespace net {
namespace http {


class WebSocket::Delegate {
public:
    virtual void OnTextMessage(WebSocket * ws, const std::string & text) = 0;
    virtual void OnClosed(WebSocket * ws) = 0;

protected:
    Delegate() {}
    virtual ~Delegate() {}
};


}  // namespace http
}  // namespace net


#endif  // YGG_NET_HTTP_WEBSOCKET_DELEGATE_H_
