#ifndef YGG_SERVER_HTTP_SERVER_WEBSOCKET_CONNECTION_H_
#define YGG_SERVER_HTTP_SERVER_WEBSOCKET_CONNECTION_H_

#include "core/base_delegate.h"
#include "core/connection.h"
#include <string>


struct mg_connection;


namespace ygg {
namespace server {


class WebSocket : public Connection {
 public:
  static Connection::Ptr New(struct mg_connection * conn,
                             const std::string & uri);
  ~WebSocket();

  virtual size_t SendText(const Text & text) const;
  virtual size_t SendBinary(const Bytes & bytes) const;
  virtual void Close();

  const char * uri() const;

 private:
  WebSocket(struct mg_connection * conn, const std::string & uri);


 private:
  struct mg_connection * conn_;
  const std::string uri_;
};


}  // namespace server
}  // namespace ygg


#endif  // YGG_SERVER_HTTP_SERVER_WEBSOCKET_CONNECTION_H_