#ifndef YGG_NET_HTTP_WEBSOCKET_H_
#define YGG_NET_HTTP_WEBSOCKET_H_

#include <string>
#include <cstdint>


struct mg_connection;


namespace net {
namespace http {


class WebSocket {
public:
    class Impl;
    class Delegate;

public:
    explicit WebSocket(mg_connection * conn);
    explicit WebSocket(Impl * pimpl);
    ~WebSocket();

    size_t SendTextMessage(const std::string & text) const;
    size_t SendBinaryMessage(const uint8_t bytes[], size_t size) const;

    void BindDelegate(Delegate * delegate);
    void UnbindDelegate();

    void FireOnTextMessageEvent(const std::string & text);
    void FireOnClosedEvent();


private:
    Impl * pimpl_;
};


}  // namespace http
}  // namespace net


#endif  // YGG_NET_HTTP_WEBSOCKET_H_
