#ifndef YGG_NET_HTTP_WEBSOCKET_IMPL_H_
#define YGG_NET_HTTP_WEBSOCKET_IMPL_H_

#include "http_websocket.h"


namespace net {
namespace http {


class WebSocket::Impl {
public:
    explicit Impl(struct mg_connection * conn = nullptr);
    virtual ~Impl();

    virtual size_t SendTextMessage(const std::string & text) const;
    virtual size_t SendBinaryMessage(const uint8_t bytes[], size_t size) const;


private:
    mg_connection * conn_;
};


}  // namespace net
}  // namespace http


#endif  // YGG_NET_HTTP_WEBSOCKET_IMPL_H_
