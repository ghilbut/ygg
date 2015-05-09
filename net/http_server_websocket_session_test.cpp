#include <gmock/gmock.h>

//#include "http_websocket.h"
#include "http_server_websocket_session_impl.h"
#include "http_server_websocket_session_delegate.h"

using namespace net::http::server::websocket;
using ::testing::_;


class SessionTest : public ::testing::Test {
};


class SessionDelegateMock : public Session::Delegate {
public:
    MOCK_METHOD2(OnTextMessage, void(Session*, const std::string&));
    MOCK_METHOD1(OnClosed, void(Session*));
};


class FakeSessionImpl : public Session::Impl {
public:
    FakeSessionImpl() {}
    ~FakeSessionImpl() {}

    size_t SendTextMessage(const std::string & text) const {
        return text.length();
    }

    size_t SendBinaryMessage(const uint8_t bytes[], size_t size) const {
        return size;
    }

    void Close() {}
};


TEST_F(SessionTest, test_bind_delegate_and_fire_events) {

    const char * const expected = "second";

    Session::Impl * pimpl = new FakeSessionImpl();
    Session ws(pimpl);

    SessionDelegateMock mock;
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

TEST_F(SessionTest, test_w) {
}
