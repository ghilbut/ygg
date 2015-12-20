#include <gmock/gmock.h>

#include "local_connection.h"
#include "test/mock.h"
#include <cstdlib>
#include <ctime>


using ::testing::_;
using ::testing::ElementsAreArray;

using namespace ygg::test;


namespace ygg {


class LocalConnectionTest : public ::testing::Test {
public:
    LocalConnectionTest() {}
    ~LocalConnectionTest() {}

    virtual void SetUp() {

        srand((unsigned int) time(nullptr));

        static const std::string kAlphaDigit(
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789");

        const int size = 100 + (rand() % 100);
        for (int i = 0; i < size; ++i) {
            const int r = rand() % kAlphaDigit.length();
            const char c = kAlphaDigit[r-1];
            text_.push_back(c);
            bytes_.push_back(c);
        }
    }

    virtual void TearDown() {
        // nothing
    }

public:
    std::string text_;
    std::vector<uint8_t> bytes_;
};



TEST_F(LocalConnectionTest, return_binded_connection_when_call_other) {

    Connection::Ptr conn(LocalConnection::New());
    ASSERT_NE(nullptr, ((LocalConnection*) conn.get())->other());
}

TEST_F(LocalConnectionTest, send_text) {

    const Text kExpected(text_);

    Connection::Ptr conn(LocalConnection::New());
    Connection::Ptr other(((LocalConnection*) conn.get())->other());

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnText(other.get(), kExpected));

    other->BindDelegate(&mock);
    conn->SendText(text_);
}

TEST_F(LocalConnectionTest, send_binary) {

    const auto kExpected = ElementsAreArray(&bytes_[0], bytes_.size());

    Connection::Ptr conn(LocalConnection::New());
    Connection::Ptr other(((LocalConnection*) conn.get())->other());

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnBinary(other.get(), kExpected));

    other->BindDelegate(&mock);
    conn->SendBinary(bytes_);
}

TEST_F(LocalConnectionTest, close) {

    Connection::Ptr conn(LocalConnection::New());
    Connection::Ptr other(((LocalConnection*) conn.get())->other());

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnClosed(conn.get()));
    EXPECT_CALL(mock, OnClosed(other.get()));

    conn->BindDelegate(&mock);
    other->BindDelegate(&mock);
    conn->Close();
}

TEST_F(LocalConnectionTest, recv_text) {

    const Text kExpected(text_);

    Connection::Ptr conn(LocalConnection::New());
    Connection::Ptr other(((LocalConnection*) conn.get())->other());

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnText(conn.get(), kExpected));

    conn->BindDelegate(&mock);
    other->SendText(text_);
}

TEST_F(LocalConnectionTest, recv_binary) {

    const auto kExpected = ElementsAreArray(&bytes_[0], bytes_.size());

    Connection::Ptr conn(LocalConnection::New());
    Connection::Ptr other(((LocalConnection*) conn.get())->other());

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnBinary(conn.get(), kExpected));

    conn->BindDelegate(&mock);
    other->SendBinary(bytes_);
}

TEST_F(LocalConnectionTest, closed) {

    Connection::Ptr conn(LocalConnection::New());
    Connection::Ptr other(((LocalConnection*) conn.get())->other());

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnClosed(conn.get()));
    EXPECT_CALL(mock, OnClosed(other.get()));

    conn->BindDelegate(&mock);
    other->BindDelegate(&mock);
    other->Close();
}


}  // namespace ygg
