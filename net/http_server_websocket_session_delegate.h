#ifndef YGG_NET_HTTP_WEBSOCKET_DELEGATE_H_
#define YGG_NET_HTTP_WEBSOCKET_DELEGATE_H_

#include "http_server_websocket_session.h"


namespace net {
namespace http {
namespace server {
namespace websocket {


class Session::Delegate {
public:
    virtual void OnTextMessage(Session * ws, const std::string & text) = 0;
    virtual void OnClosed(Session * ws) = 0;

protected:
    Delegate() {}
    virtual ~Delegate() {}
};


}  // namespace websocket
}  // namespace server
}  // namespace http
}  // namespace net


#endif  // YGG_NET_HTTP_WEBSOCKET_DELEGATE_H_
