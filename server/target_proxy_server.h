#ifndef SERVER_TARGET_PROXY_SERVER_H_
#define SERVER_TARGET_PROXY_SERVER_H_

#include "net/http_server.h"
#include "target/manager.h"


using namespace ygg::net;
using namespace ygg::server::target;


namespace ygg {
namespace server {


class TargetProxyServer : public HttpServer::Delegate {
 public:
  TargetProxyServer();
  ~TargetProxyServer();

  void Start();
  void Stop();

  virtual void OnRequest(struct mg_connection * conn,
                         struct http_message * msg);
  virtual void OnWebSocket(Connection::Ptr ws, const std::string & uri);


 private:
  HttpServer server_;
  Manager manager_;
};


}  // namespace server
}  // namespace ygg


#endif  // SERVER_TARGET_PROXY_SERVER_H_
