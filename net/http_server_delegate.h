#ifndef YGG_NET_HTTP_SERVER_DELEGATE_H_
#define YGG_NET_HTTP_SERVER_DELEGATE_H_

#include <string>


namespace net {
namespace http {


namespace server {
namespace websocket {
    class Session;
}  // namespce websocket
}  // namespace server

typedef server::websocket::Session WebSocket;


class ServerDelegate {
public:
    virtual void OnRequest() = 0;
    virtual void OnConnect(WebSocket * ws) = 0;
    virtual void OnTextMessage(WebSocket * ws, const std::string & text) = 0;
    virtual void OnClose(WebSocket * ws) = 0;

protected:
    virtual ~ServerDelegate() {}
};


}  // namespace http
}  // namespace net

#endif  // YGG_NET_HTTP_SERVER_DELEGATE_H_

