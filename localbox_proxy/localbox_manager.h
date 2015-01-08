#ifndef LOCALBOX_PROXY_LOCALBOX_MANAGER_H_
#define LOCALBOX_PROXY_LOCALBOX_MANAGER_H_

#include "http/http_server.h"
#include "http/http_server_delegate.h"
#include <map>
#include <ctime>


class LocalBox;

class LocalBoxManager : public HttpServerDelegate {
public:
    LocalBoxManager();
    ~LocalBoxManager();

    void Start();
    void Stop();

    // HttpServerDelegate
    virtual void OnRequest(void);
    virtual void OnConnect(HttpWebsocket *ws);
    virtual void OnTextMessage(HttpWebsocket *ws, const std::string& text);
    virtual void OnClose(HttpWebsocket *ws);


private:
	bool regLocalBox(HttpWebsocket* ws, const std::string& text);
	void unregLocalBox(HttpWebsocket* ws);



private:
	// NOTE(ghilbut): check timeout from ready time
	std::map<HttpWebsocket*, time_t> readyTable_;
	std::map<HttpWebsocket*, LocalBox*> localboxTable_;
    HttpServer server_;
};

#endif  // LOCALBOX_PROXY_MANAGER_H_