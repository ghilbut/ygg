#ifndef YGG_NET_HTTP_SERVER_NULL_DELEGATE_H_
#define YGG_NET_HTTP_SERVER_NULL_DELEGATE_H_

#include "http_server.h"
#include "http_server_delegate.h"
#include <string>


namespace net {
namespace http {


class HttpServer::NullDelegate : public ServerDelegate {
public:
	void OnRequest();
	void OnConnect(WebSocket * ws);
    void OnTextMessage(WebSocket * ws, const std::string & text);
    void OnClose(WebSocket * ws);
};


}  // namespace http
}  // namespace net

#endif  // LOCALBOX_PROXY_HTTP_SERVER_DELEGATE_H_
