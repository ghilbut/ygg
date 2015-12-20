#include <gmock/gmock.h>

#include "connection.h"
#include "test/fake.h"

using ::testing::_;
using ::testing::ElementsAreArray;


namespace ygg {


class MockConnectionDelegate : public Connection::Delegate {
public:
    MOCK_METHOD2(OnText, void(Connection*, const Text&));
    MOCK_METHOD2(OnBinary, void(Connection*, const Bytes&));
    MOCK_METHOD1(OnClosed, void(Connection*));
};


TEST(ConnectionTest, fire_text_event) {

    static const Text kText("second");
    static const Text kExpected(kText);

    Connection::Ptr conn(test::FakeConnection::New());

    MockConnectionDelegate mock;
    EXPECT_CALL(mock, OnText(conn.get(), kExpected)).Times(2);

    conn->FireOnTextEvent("first");

    conn->BindDelegate(&mock);
    conn->FireOnTextEvent(kText);
    conn->FireOnTextEvent(kText);
    conn->UnbindDelegate();

    conn->FireOnTextEvent("third");
}

TEST(ConnectionTest, fire_binary_event) {

    static const uint8_t kData[10] = {0,1,2,3,4,5,6,7,8,9};
    static const Bytes kBuffer(kData, kData+10);
    static const auto kExpected = ElementsAreArray(kData, 10);

    Connection::Ptr conn(test::FakeConnection::New());

    MockConnectionDelegate mock;
    EXPECT_CALL(mock, OnBinary(conn.get(), kExpected)).Times(2);

    conn->FireOnBinaryEvent(kBuffer);

    conn->BindDelegate(&mock);
    conn->FireOnBinaryEvent(kBuffer);
    conn->FireOnBinaryEvent(kBuffer);
    conn->UnbindDelegate();

    conn->FireOnBinaryEvent(kBuffer);
}

TEST(ConnectionTest, fire_close_event) {

    Connection::Ptr conn(test::FakeConnection::New());

    MockConnectionDelegate mock;
    EXPECT_CALL(mock, OnClosed(conn.get())).Times(2);

    conn->FireOnClosedEvent();

    conn->BindDelegate(&mock);
    conn->FireOnClosedEvent();
    conn->FireOnClosedEvent();
    conn->UnbindDelegate();

    conn->FireOnClosedEvent();
}


}  // namespace ygg
