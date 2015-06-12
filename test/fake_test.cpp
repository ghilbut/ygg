#include <gmock/gmock.h>

#include "fake.h"
#include "mock.h"
#include <cstdlib>
#include <ctime>


using ::testing::_;
using ::testing::ElementsAreArray;


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
    EXPECT_CALL(mock, constructed()).Times(6);
    EXPECT_CALL(mock, destructed()).Times(6);

    {
        Connection::Ptr conn(FakeConnection::New(&mock));
        Session::Ptr session(FakeSession::New(&mock));
    }

    {
        Connection::Ptr conn(FakeConnection::New(&mock));
        Session::Ptr session(FakeSession::New(conn.get(), &mock));
    }

    {
        Session::Ptr session(FakeSession::New(&mock));
        Connection::Ptr conn(FakeConnection::New(session.get(), &mock));
    }
}

TEST_F(FakeTest, test_fake_connection_has_default_session) {

    Connection::Ptr conn(FakeConnection::New());
    ASSERT_NE(nullptr, ((FakeConnection*) conn.get())->session());
}

TEST_F(FakeTest, test_fake_connection_can_bind_external_session) {

    Session::Ptr session(FakeSession::New());
    Connection::Ptr conn(FakeConnection::New(session.get()));
    ASSERT_EQ(session, ((FakeConnection*) conn.get())->session());
}

TEST_F(FakeTest, test_fake_session_has_default_connection) {

    Session::Ptr session(FakeSession::New());
    ASSERT_NE(nullptr, ((FakeSession*) session.get())->conn());
}

TEST_F(FakeTest, test_fake_session_can_bind_external_connection) {

    Connection::Ptr conn(FakeConnection::New());
    Session::Ptr session(FakeSession::New(conn.get()));
    ASSERT_EQ(conn, ((FakeSession*) session.get())->conn());
}

TEST_F(FakeTest, test_fake_connection_send_to_its_session) {

    const std::string kText(expected_text_);
    const std::vector<uint8_t> kBytes(expected_bytes_);
    const auto check_bytes = ElementsAreArray(&expected_bytes_[0], expected_bytes_.size());

    Connection::Ptr conn(FakeConnection::New());
    Session::Ptr session(((FakeConnection*) conn.get())->session());

    SessionDelegateMock mock;
    EXPECT_CALL(mock, OnText(session.get(), expected_text_));
    EXPECT_CALL(mock, OnBinary(session.get(), check_bytes));
    EXPECT_CALL(mock, OnClosed(session.get())).Times(2);

    session->BindDelegate(&mock);
    conn->SendText(kText);
    conn->SendBinary(kBytes);
    conn->Close();

    session->Close();
}

TEST_F(FakeTest, test_fake_connection_receive_from_its_session) {

    const std::string kText(expected_text_);
    const std::vector<uint8_t> kBytes(expected_bytes_);
    const auto check_bytes = ElementsAreArray(&expected_bytes_[0], expected_bytes_.size());

    Connection::Ptr conn(FakeConnection::New());
    Session::Ptr session(((FakeConnection*) conn.get())->session());

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnOpened(conn.get()));
    EXPECT_CALL(mock, OnText(conn.get(), expected_text_));
    EXPECT_CALL(mock, OnBinary(conn.get(), check_bytes));
    EXPECT_CALL(mock, OnClosed(conn.get())).Times(2);

    conn->BindDelegate(&mock);
    conn->Open();
    session->SendText(kText);
    session->SendBinary(kBytes);
    session->Close();

    conn->Close();
}

TEST_F(FakeTest, test_fake_session_send_to_its_connection) {

    const std::string kText(expected_text_);
    const std::vector<uint8_t> kBytes(expected_bytes_);
    const auto check_bytes = ElementsAreArray(&expected_bytes_[0], expected_bytes_.size());

    Session::Ptr session(FakeSession::New());
    Connection::Ptr conn(((FakeSession*) session.get())->conn());

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnOpened(conn.get()));
    EXPECT_CALL(mock, OnText(conn.get(), expected_text_));
    EXPECT_CALL(mock, OnBinary(conn.get(), check_bytes));
    EXPECT_CALL(mock, OnClosed(conn.get())).Times(2);

    conn->BindDelegate(&mock);
    conn->Open();
    session->SendText(kText);
    session->SendBinary(kBytes);
    session->Close();

    conn->Close();
}

TEST_F(FakeTest, test_fake_session_receive_from_its_connection) {

    const std::string kText(expected_text_);
    const std::vector<uint8_t> kBytes(expected_bytes_);
    const auto check_bytes = ElementsAreArray(&expected_bytes_[0], expected_bytes_.size());

    Session::Ptr session(FakeSession::New());
    Connection::Ptr conn(((FakeSession*) session.get())->conn());

    SessionDelegateMock mock;
    EXPECT_CALL(mock, OnText(session.get(), expected_text_));
    EXPECT_CALL(mock, OnBinary(session.get(), check_bytes));
    EXPECT_CALL(mock, OnClosed(session.get())).Times(2);

    session->BindDelegate(&mock);
    conn->SendText(kText);
    conn->SendBinary(kBytes);
    conn->Close();

    session->Close();
}


}  // namespace test
