#ifndef NET_HTTP_SERVER_WEBSOCKET_CONNECTION_H_
#define NET_HTTP_SERVER_WEBSOCKET_CONNECTION_H_

#include "core/base_delegate.h"
#include "core/connection.h"
#include <string>


struct mg_connection;


namespace ygg {
namespace net {


class WebSocket : public Connection {
 public:
  static Connection::Ptr New(struct mg_connection * conn);
  ~WebSocket();

  virtual size_t SendText(const Text & text) const;
  virtual size_t SendBinary(const Bytes & bytes) const;
  virtual void Close();

 private:
  WebSocket(struct mg_connection * conn);


 private:
  struct mg_connection * conn_;
  const std::string uri_;
};


}  // namespace net
}  // namespace ygg


#endif  // NET_HTTP_SERVER_WEBSOCKET_CONNECTION_H_
