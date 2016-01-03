#ifndef YGG_SERVER_HTTP_SERVER_WEBSOCKET_CONNECTION_H_
#define YGG_SERVER_HTTP_SERVER_WEBSOCKET_CONNECTION_H_

#include "core/base_delegate.h"
#include "core/connection.h"


struct mg_connection;


namespace ygg {
namespace server {


class WebSocket : public Connection {
 public:
  static Connection::Ptr New(struct mg_connection * conn);
  ~WebSocket();

  void BindDelegate(Delegate * delegate);
  void UnbindDelegate();

  virtual size_t SendText(const Text & text) const;
  virtual size_t SendBinary(const Bytes & bytes) const;
  virtual void Close();

 private:
  WebSocket(struct mg_connection * conn);


 private:
  struct mg_connection * conn_;
  Delegate * delegate_;
};


}  // namespace server
}  // namespace ygg


#endif  // YGG_SERVER_HTTP_SERVER_WEBSOCKET_CONNECTION_H_
