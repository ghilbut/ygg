#include "http_server_websocket_connection.h"
#include <mongoose/mongoose.h>
#include <cassert>


namespace ygg {
namespace net {


class NullWebSocketDelegate : public Connection::Delegate {
 public:
  virtual void OnText(Connection*, const Text&) {}
  virtual void OnBinary(Connection*, const Bytes&) {}
  virtual void OnClosed(Connection*) {}
};

static NullWebSocketDelegate kNullDelegate;


Connection::Ptr WebSocket::New(struct mg_connection * conn) {
  assert(conn != nullptr);
  return new WebSocket(conn);
}

WebSocket::~WebSocket() {
  // nothing
}

size_t WebSocket::SendText(const Text & text) const {
  mg_send_websocket_frame(conn_, WEBSOCKET_OP_TEXT, &text[0], text.length());
  return 0;
}

size_t WebSocket::SendBinary(const Bytes & bytes) const {
  mg_send_websocket_frame(conn_, WEBSOCKET_OP_BINARY, &bytes[0], bytes.size());
  return 0;
}

void WebSocket::Close() {
  mg_send_websocket_frame(conn_, WEBSOCKET_OP_CLOSE, nullptr, 0);
}

WebSocket::WebSocket(struct mg_connection * conn) 
  : Connection(), conn_(conn) {
  // nothing
}


}  // namespace net
}  // namespace ygg
