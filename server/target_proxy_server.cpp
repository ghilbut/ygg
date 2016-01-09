#include "target_proxy_server.h"


namespace ygg {
namespace server {


TargetProxyServer::TargetProxyServer() {
  server_.BindDelegate(this);
}

TargetProxyServer::~TargetProxyServer() {
  server_.UnbindDelegate();
}

void TargetProxyServer::Start() {
  server_.Start();
}

void TargetProxyServer::Stop() {
  server_.Stop();
}

void TargetProxyServer::OnRequest(struct mg_connection * conn,
                                  struct http_message * msg) {
}

void TargetProxyServer::OnWebSocket(Connection::Ptr ws,
                                    const std::string & uri) {
  if (uri == "/ctrl") {
    manager_.SetCtrlConnection(ws);
    return;
  }

  if (uri == "/target") {
    manager_.SetTargetConnection(ws);
    return;
  }
}


}  // namespace server
}  // namespace ygg
