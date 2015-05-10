#include <gmock/gmock.h>

#include "session.h"
#include "session_delegate.h"

using ::testing::_;


namespace net {


class SessionTest : public ::testing::Test {
};


class SessionDelegateMock : public Session::Delegate {
public:
    MOCK_METHOD2(OnText, void(Session*, const std::string&));
    MOCK_METHOD3(OnBinary, void(Session*, const uint8_t[], size_t));
    MOCK_METHOD1(OnClosed, void(Session*));
};


class FakeSession : public Session {
public:
    FakeSession() {}
    ~FakeSession() {}

    size_t SendText(const std::string & text) const {
        return text.length();
    }

    size_t SendBinary(const uint8_t bytes[], size_t size) const {
        return size;
    }

    void Close() {}
};


TEST_F(SessionTest, test_bind_delegate_and_fire_events) {

    const char * const expected = "second";

    FakeSession session;

    SessionDelegateMock mock;
    EXPECT_CALL(mock, OnText(&session, expected)).Times(2);
    EXPECT_CALL(mock, OnBinary(&session, _, _).Times(2);
    EXPECT_CALL(mock, OnClosed(&session)).Times(2);

    static const uint8_t buffer[10] = {0,};

    session.FireOnTextEvent("first");
    session.FirOnBinaryEvent(buffer, 10);
    session.FirOnBinaryEvent(buffer, 10);
    session.FireOnClosedEvent();

    session.BindDelegate(&mock);
    session.FireOnTextEvent(expected);
    session.FirOnBinaryEvent(buffer, 10);
    session.FireOnClosedEvent();
    session.FireOnTextEvent(expected);
    session.FirOnBinaryEvent(buffer, 10);
    session.FireOnClosedEvent();
    session.UnbindDelegate();

    session.FireOnTextEvent("third");
    session.FirOnBinaryEvent(buffer, 10);
    session.FireOnClosedEvent();
}


}  // namespace net