#include <gmock/gmock.h>

#include "connection.h"
#include "test/fake.h"

using ::testing::_;
using ::testing::ElementsAreArray;


namespace codebase {


class ConnectionTest : public ::testing::Test {
};


class ConnectionDelegateMock : public Connection::Delegate {
public:
    MOCK_METHOD2(OnText, void(Connection*, const std::string&));
    MOCK_METHOD2(OnBinary, void(Connection*, const std::vector<uint8_t>&));
    MOCK_METHOD1(OnClosed, void(Connection*));
};


TEST_F(ConnectionTest, test_bind_delegate_and_fire_events) {

    static const uint8_t kExpectedBuffer[10] = {0,};

    static const std::string kText = "second";
    static const std::vector<uint8_t> kBuffer(kExpectedBuffer, kExpectedBuffer+10);

    Connection::Ptr conn(test::FakeConnection::New());

    ConnectionDelegateMock mock;
    Connection * p = conn.get();
    EXPECT_CALL(mock, OnText(p, kText)).Times(2);
    EXPECT_CALL(mock, OnBinary(p, ElementsAreArray(kExpectedBuffer, 10))).Times(2);
    EXPECT_CALL(mock, OnClosed(p)).Times(2);

    conn->FireOnTextEvent("first");
    conn->FireOnBinaryEvent(kBuffer);
    conn->FireOnClosedEvent();

    conn->BindDelegate(&mock);
    conn->FireOnTextEvent(kText);
    conn->FireOnBinaryEvent(kBuffer);
    conn->FireOnClosedEvent();
    conn->FireOnTextEvent(kText);
    conn->FireOnBinaryEvent(kBuffer);
    conn->FireOnClosedEvent();
    conn->UnbindDelegate();

    conn->FireOnTextEvent("third");
    conn->FireOnBinaryEvent(kBuffer);
    conn->FireOnClosedEvent();
}


}  // namespace codebase
