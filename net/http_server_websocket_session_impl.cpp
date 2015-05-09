#include "http_server_websocket_session_impl.h"

#include <mongoose.h>


namespace net {
namespace http {
namespace server {
namespace websocket {


Session::Impl::Impl(mg_connection * conn)
    : conn_(conn) {
}

Session::Impl::~Impl() {
}

size_t Session::Impl::SendTextMessage(const std::string & text) const {
    return mg_websocket_write(conn_, WEBSOCKET_OPCODE_TEXT, text.c_str(), text.length());
}

size_t Session::Impl::SendBinaryMessage(const uint8_t bytes[], size_t size) const {
    const char * data = reinterpret_cast<const char *>(bytes);
    return mg_websocket_write(conn_, WEBSOCKET_OPCODE_BINARY, data, size);
}

void Session::Impl::Close() {
    if (conn_ != nullptr) {
        mg_websocket_write(conn_, WEBSOCKET_OPCODE_CONNECTION_CLOSE, nullptr, 0);
    }
    conn_ = nullptr;
}


}  // namespace websocket
}  // namespace server
}  // namespace http
}  // namespace net
