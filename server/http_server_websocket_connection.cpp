#include "http_server_websocket_connection.h"
#include <mongoose/mongoose.h>
#include <cassert>


namespace ygg {
namespace server {


class NullWebSocketDelegate : public Connection::Delegate {
 public:
  virtual void OnText(Connection*, const Text&) {}
  virtual void OnBinary(Connection*, const Bytes&) {}
  virtual void OnClosed(Connection*) {}
};

static NullWebSocketDelegate kNullDelegate;


Connection::Ptr WebSocket::New(struct mg_connection * conn,
                               const std::string & uri) {
  assert(conn != nullptr);
  return new WebSocket(conn, uri);
}

WebSocket::~WebSocket() {
  // nothing
}

size_t WebSocket::SendText(const Text & text) const {
  mg_send_websocket_frame(conn_, WEBSOCKET_OP_TEXT, &text[0], text.length());
  return 0;
}

size_t WebSocket::SendBinary(const Bytes & bytes) const {
  return 0;
}

void WebSocket::Close() {
  mg_send_websocket_frame(conn_, WEBSOCKET_OP_CLOSE, nullptr, 0);
}

const char * WebSocket::uri() const {
  return uri_.c_str();
}

WebSocket::WebSocket(struct mg_connection * conn, const std::string & uri) 
  : Connection(), conn_(conn), uri_(uri) {
  // nothing
}


}  // namespace server
}  // namespace ygg
