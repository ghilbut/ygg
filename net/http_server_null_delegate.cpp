#include "http_server_null_delegate.h"
#include "http_server_websocket_session.h"


namespace net {
namespace http {


void HttpServer::NullDelegate::OnRequest() {
	// TODO(ghilbut): null state handler
};

void HttpServer::NullDelegate::OnConnect(WebSocket * ws) {
	ws->Close();
}

void HttpServer::NullDelegate::OnTextMessage(WebSocket * ws, const std::string & text) {
	ws->Close();
}

void HttpServer::NullDelegate::OnClose(WebSocket * ws) {
	// nothing
}


}  // namespace http
}  // namespace net
