#ifndef YGG_HTTP_SERVER_WEBSOCKET_SESSION_IMPL_H_
#define YGG_HTTP_SERVER_WEBSOCKET_SESSION_IMPL_H_

#include "http_server_websocket_session.h"


namespace net {
namespace http {
namespace server {
namespace websocket {


class Session::Impl {
public:
    explicit Impl(mg_connection * conn = nullptr);
    virtual ~Impl();

    virtual size_t SendTextMessage(const std::string & text) const;
    virtual size_t SendBinaryMessage(const uint8_t bytes[], size_t size) const;
    virtual void Close();


private:
    mg_connection * conn_;
};


}  // namespace websocket
}  // namespace server
}  // namespace http
}  // namespace net


#endif  // YGG_HTTP_SERVER_WEBSOCKET_SESSION_IMPL_H_
