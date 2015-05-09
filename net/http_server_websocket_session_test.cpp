#include <gmock/gmock.h>

//#include "http_websocket.h"
#include "http_websocket_impl.h"
#include "http_websocket_delegate.h"

using namespace net::http;
using ::testing::_;


class WebSocketTest : public ::testing::Test {
};


class WebSocketDelegateMock : public WebSocket::Delegate {
public:
    MOCK_METHOD2(OnTextMessage, void(WebSocket*, const std::string&));
    MOCK_METHOD1(OnClosed, void(WebSocket*));
};


class FakeWebSocketImpl : public WebSocket::Impl {
public:
    FakeWebSocketImpl() {}
    ~FakeWebSocketImpl() {}
};


TEST_F(WebSocketTest, test_bind_delegate_and_fire_events) {

    const char * const expected = "second";

    WebSocket::Impl * pimpl = new FakeWebSocketImpl();
    WebSocket ws(pimpl);

    WebSocketDelegateMock mock;
    EXPECT_CALL(mock, OnTextMessage(&ws, expected)).Times(2);
    EXPECT_CALL(mock, OnClosed(&ws)).Times(2);

    ws.FireOnTextMessageEvent("first");
    ws.FireOnClosedEvent();

    ws.BindDelegate(&mock);
    ws.FireOnTextMessageEvent(expected);
    ws.FireOnClosedEvent();
    ws.FireOnTextMessageEvent(expected);
    ws.FireOnClosedEvent();
    ws.UnbindDelegate();

    ws.FireOnTextMessageEvent("third");
    ws.FireOnClosedEvent();
}

TEST_F(WebSocketTest, test_w) {
}
