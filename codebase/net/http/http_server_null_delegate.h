#ifndef YGG_SERVER_HTTP_SERVER_NULL_DELEGATE_H_
#define YGG_SERVER_HTTP_SERVER_NULL_DELEGATE_H_

#include "http_server.h"
#include "http_server_delegate.h"
#include <string>


namespace codebase {


class HttpServer::NullDelegate : public HttpServerDelegate {
public:
	void OnRequest(void);
	void OnConnect(HttpWebsocket &ws);
    void OnTextMessage(HttpWebsocket &ws, const std::string &text);
    void OnClose(HttpWebsocket &ws);
};


}  // namespace codebase

#endif  // LOCALBOX_PROXY_HTTP_SERVER_DELEGATE_H_