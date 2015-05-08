#include "http_websocket_impl.h"

#include <mongoose.h>


namespace net {
namespace http {


WebSocket::Impl::Impl(struct mg_connection * conn)
    : conn_(conn) {
}

WebSocket::Impl::~Impl() {
}

size_t WebSocket::Impl::SendTextMessage(const std::string & text) const {
    return mg_websocket_write(conn_, WEBSOCKET_OPCODE_TEXT, text.c_str(), text.length());
}

size_t WebSocket::Impl::SendBinaryMessage(const uint8_t bytes[], size_t size) const {
    const char * data = reinterpret_cast<const char *>(bytes);
    return mg_websocket_write(conn_, WEBSOCKET_OPCODE_BINARY, data, size);
}


}  // namespace net
}  // namespace http
