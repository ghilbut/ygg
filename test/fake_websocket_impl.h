#ifndef YGG_TEST_FAKE_WEBSOCKET_IMPL_H_
#define YGG_TEST_FAKE_WEBSOCKET_IMPL_H_

#include "net/http_websocket_impl.h"


namespace test {
namespace fake {


class WebSocketImpl : public net::http::WebSocket::Impl {
public:
    WebSocketImpl() {}
    ~WebSocketImpl() {}
};


}  // namespace fake
}  // namespace test


#endif  // YGG_TEST_FAKE_WEBSOCKET_IMPL_H_
