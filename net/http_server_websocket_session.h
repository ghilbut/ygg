#ifndef YGG_NET_HTTP_SERVER_WEBSOCKET_SESSION_H_
#define YGG_NET_HTTP_SERVER_WEBSOCKET_SESSION_H_

#include <string>
#include <cstdint>


struct mg_connection;


namespace net {
namespace http {
namespace server {
namespace websocket {


class Session {
public:
    class Impl;
    class Delegate;

public:
    explicit Session(mg_connection * conn);
    explicit Session(Impl * pimpl);
    ~Session();

    size_t SendTextMessage(const std::string & text) const;
    size_t SendBinaryMessage(const uint8_t bytes[], size_t size) const;
    void Close();

    void BindDelegate(Delegate * delegate);
    void UnbindDelegate();

    void FireOnTextMessageEvent(const std::string & text);
    void FireOnClosedEvent();


private:
    Impl * pimpl_;
    Delegate * delegate_;
};


}  // namespace websocket
}  // namespace server
}  // namespace http
}  // namespace net


#endif  // YGG_NET_HTTP_SERVER_WEBSOCKET_SESSION_H_
