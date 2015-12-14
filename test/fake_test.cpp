#include <gmock/gmock.h>

#include "fake.h"
#include "mock.h"
#include <cstdlib>
#include <ctime>


using ::testing::_;
using ::testing::ElementsAreArray;


namespace ygg {
namespace test {


class FakeTest : public ::testing::Test {
public:
    FakeTest() {}
    ~FakeTest() {}

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
            expected_text_.push_back(c);
            expected_bytes_.push_back(c);
        }
    }

    virtual void TearDown() {
        // nothing
    }

public:
    std::string expected_text_;
    std::vector<uint8_t> expected_bytes_;
};


TEST_F(FakeTest, test_life_cycle) {

    LifeCycleMock mock;
    EXPECT_CALL(mock, constructed()).Times(4);
    EXPECT_CALL(mock, destructed()).Times(4);

    {
        Connection::Ptr conn0(FakeConnection::New(&mock));
        Connection::Ptr conn1(FakeConnection::New(&mock));
    }

    {
        Connection::Ptr conn0(FakeConnection::New(&mock));
        Connection::Ptr conn1(FakeConnection::New(conn0.get(), &mock));
    }
}

TEST_F(FakeTest, test_fake_connection_has_default_connection) {

    Connection::Ptr conn(FakeConnection::New());
    ASSERT_NE(nullptr, ((FakeConnection*) conn.get())->conn());
}

TEST_F(FakeTest, test_fake_connection_can_bind_external_connection) {

    Connection::Ptr ext(FakeConnection::New());
    Connection::Ptr conn(FakeConnection::New(ext.get()));
    ASSERT_EQ(ext, ((FakeConnection*) conn.get())->conn());
}

TEST_F(FakeTest, test_fake_connection_send_to_its_connection) {

    const std::string kText(expected_text_);
    const std::vector<uint8_t> kBytes(expected_bytes_);
    const auto check_bytes = ElementsAreArray(&expected_bytes_[0], expected_bytes_.size());

    Connection::Ptr conn(FakeConnection::New());
    Connection::Ptr child(((FakeConnection*) conn.get())->conn());

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnText(child.get(), expected_text_));
    EXPECT_CALL(mock, OnBinary(child.get(), check_bytes));
    EXPECT_CALL(mock, OnClosed(child.get())).Times(2);

    child->BindDelegate(&mock);
    conn->SendText(kText);
    conn->SendBinary(kBytes);
    conn->Close();

    child->Close();
}

TEST_F(FakeTest, test_fake_connection_receive_from_its_connection) {

    const std::string kText(expected_text_);
    const std::vector<uint8_t> kBytes(expected_bytes_);
    const auto check_bytes = ElementsAreArray(&expected_bytes_[0], expected_bytes_.size());

    Connection::Ptr conn(FakeConnection::New());
    Connection::Ptr child(((FakeConnection*) conn.get())->conn());

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnText(conn.get(), expected_text_));
    EXPECT_CALL(mock, OnBinary(conn.get(), check_bytes));
    EXPECT_CALL(mock, OnClosed(conn.get())).Times(2);

    conn->BindDelegate(&mock);
    child->SendText(kText);
    child->SendBinary(kBytes);
    child->Close();

    conn->Close();
}


}  // namespace test
}  // namespace ygg
