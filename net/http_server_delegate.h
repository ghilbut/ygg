#ifndef YGG_NET_HTTP_SERVER_DELEGATE_H_
#define YGG_NET_HTTP_SERVER_DELEGATE_H_

#include <string>


namespace net {
namespace http {


typedef server::websocket::session WebSocket;

class ServerDelegate {
public:
    virtual void OnRequest(void) = 0;
    virtual void OnConnect(Websocket * ws) = 0;
    virtual void OnTextMessage(Websocket * ws, const std::string & text) = 0;
    virtual void OnClose(Websocket * ws) = 0;

protected:
    virtual ~ServerDelegate() {}
};


}  // namespace http
}  // namespace net

#endif  // YGG_NET_HTTP_SERVER_DELEGATE_H_

