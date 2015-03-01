#ifndef LOCALBOX_PROXY_LOCALBOX_MANAGER_H_
#define LOCALBOX_PROXY_LOCALBOX_MANAGER_H_

#include "codebase/net/http/http_server.h"
#include "codebase/net/http/http_server_delegate.h"
#include <map>
#include <ctime>


class LocalBox;
typedef boost::shared_ptr<LocalBox> LocalBoxPtr;

class LocalBoxManager : public codebase::HttpServerDelegate {
public:
    LocalBoxManager();
    ~LocalBoxManager();

    void Start();
    void Stop();

    // HttpServerDelegate
    virtual void OnRequest(void);
	virtual void OnConnect(codebase::HttpWebsocket &ws);
	virtual void OnTextMessage(codebase::HttpWebsocket &ws, const std::string &text);
	virtual void OnClose(codebase::HttpWebsocket &ws);


private:
	bool regLocalBox(codebase::HttpWebsocket &ws, const std::string &text);
	void unregLocalBox(codebase::HttpWebsocket &ws);



private:
	// NOTE(ghilbut): check timeout from ready time
	std::map<codebase::HttpWebsocket, time_t> readyTable_;
	std::map<codebase::HttpWebsocket, LocalBox> localboxTable_;
    codebase::HttpServer server_;
};

#endif  // LOCALBOX_PROXY_MANAGER_H_