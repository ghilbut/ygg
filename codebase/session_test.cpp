#include <gmock/gmock.h>

#include "session.h"
#include "session_delegate.h"
#include "test/fake.h"

using ::testing::_;


namespace codebase {


class SessionTest : public ::testing::Test {
};


class SessionDelegateMock : public Session::Delegate {
public:
    MOCK_METHOD2(OnText, void(Session *, const std::string &));
    MOCK_METHOD3(OnBinary, void(Session *, const uint8_t[], size_t));
    MOCK_METHOD1(OnClosed, void(Session *));
};


TEST_F(SessionTest, test_bind_delegate_and_fire_events) {

    const char * const expected = "second";

    Session::Ptr session_ptr(test::FakeSession::New());
    Session * session = session_ptr.get();

    static const uint8_t buffer[10] = {0,};

    SessionDelegateMock mock;
    EXPECT_CALL(mock, OnText(session, expected)).Times(2);
    EXPECT_CALL(mock, OnBinary(session, buffer, 10)).Times(2);
    EXPECT_CALL(mock, OnClosed(session)).Times(2);

    session->FireOnTextEvent("first");
    session->FireOnBinaryEvent(buffer, 10);
    session->FireOnClosedEvent();

    session->BindDelegate(&mock);
    session->FireOnTextEvent(expected);
    session->FireOnBinaryEvent(buffer, 10);
    session->FireOnClosedEvent();
    session->FireOnTextEvent(expected);
    session->FireOnBinaryEvent(buffer, 10);
    session->FireOnClosedEvent();
    session->UnbindDelegate();

    session->FireOnTextEvent("third");
    session->FireOnBinaryEvent(buffer, 10);
    session->FireOnClosedEvent();
}


}  // namespace codebase
