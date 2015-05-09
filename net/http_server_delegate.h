#ifndef LOCALBOX_PROXY_HTTP_SERVER_DELEGATE_H_
#define LOCALBOX_PROXY_HTTP_SERVER_DELEGATE_H_

#include <string>


namespace codebase {


class HttpWebsocket;

class HttpServerDelegate {
public:
    virtual void OnRequest(void) = 0;
    virtual void OnConnect(HttpWebsocket &ws) = 0;
    virtual void OnTextMessage(HttpWebsocket &ws, const std::string &text) = 0;
    virtual void OnClose(HttpWebsocket &ws) = 0;
};


}  // namespace codebase

#endif  // LOCALBOX_PROXY_HTTP_SERVER_DELEGATE_H_