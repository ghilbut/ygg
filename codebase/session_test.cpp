#include <gmock/gmock.h>

#include "session.h"
#include "session_delegate.h"
#include "test/fake.h"

using ::testing::_;
using ::testing::ElementsAreArray;


namespace codebase {


class SessionTest : public ::testing::Test {
};


class SessionDelegateMock : public Session::Delegate {
public:
    MOCK_METHOD2(OnText, void(Session*, const std::string&));
    MOCK_METHOD2(OnBinary, void(Session*, const std::vector<uint8_t>&));
    MOCK_METHOD1(OnClosed, void(Session*));
};


TEST_F(SessionTest, test_bind_delegate_and_fire_events) {

    static const uint8_t kExpectedBuffer[10] = {0,};

    static const std::string kText = "second";
    static const std::vector<uint8_t> kBuffer(kExpectedBuffer, kExpectedBuffer+10);

    Session::Ptr session_ptr(test::FakeSession::New());
    Session * session = session_ptr.get();

    SessionDelegateMock mock;
    EXPECT_CALL(mock, OnText(session, kText)).Times(2);
    EXPECT_CALL(mock, OnBinary(session, ElementsAreArray(kExpectedBuffer, 10))).Times(2);
    EXPECT_CALL(mock, OnClosed(session)).Times(2);

    session->FireOnTextEvent("first");
    session->FireOnBinaryEvent(kBuffer);
    session->FireOnClosedEvent();

    session->BindDelegate(&mock);
    session->FireOnTextEvent(kText);
    session->FireOnBinaryEvent(kBuffer);
    session->FireOnClosedEvent();
    session->FireOnTextEvent(kText);
    session->FireOnBinaryEvent(kBuffer);
    session->FireOnClosedEvent();
    session->UnbindDelegate();

    session->FireOnTextEvent("third");
    session->FireOnBinaryEvent(kBuffer);
    session->FireOnClosedEvent();
}


}  // namespace codebase
