#include "http_server_null_delegate.h"
#include "http_websocket.h"

namespace codebase {


void HttpServer::NullDelegate::OnRequest(void) {
	// TODO(ghilbut): null state handler
};

void HttpServer::NullDelegate::OnConnect(HttpWebsocket &ws) {
	ws.Close();
}

void HttpServer::NullDelegate::OnTextMessage(HttpWebsocket &ws, const std::string &text) {
	ws.Close();
}

void HttpServer::NullDelegate::OnClose(HttpWebsocket &ws) {
	// nothing
}


}  // namespace codebase