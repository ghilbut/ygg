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
        Connection::Ptr conn(new FakeConnection(&mock));
        Session::Ptr session(new FakeSession(&mock));
    }

    {
        Connection::Ptr conn(new FakeConnection(&mock));
        Session::Ptr session(new FakeSession(conn.get(), &mock));
    }

    {
        Session::Ptr session(new FakeSession(&mock));
        Connection::Ptr conn(new FakeConnection(session.get(), &mock));
    }
}

TEST_F(FakeTest, test_fake_connection_has_default_session) {

    FakeConnection * fake = new FakeConnection();
    ASSERT_NE(nullptr, fake->session());
    delete fake;
}

TEST_F(FakeTest, test_fake_connection_can_bind_external_session) {

    Session::Ptr session(new FakeSession());
    FakeConnection * fake = new FakeConnection(session.get());
    ASSERT_EQ(session, fake->session());
    delete fake;
}

TEST_F(FakeTest, test_fake_session_has_default_connection) {

    FakeSession * fake = new FakeSession();
    ASSERT_NE(nullptr, fake->conn());
    delete fake;
}

TEST_F(FakeTest, test_fake_session_can_bind_external_connection) {

    Connection::Ptr conn(new FakeConnection());
    FakeSession * fake = new FakeSession(conn.get());
    ASSERT_EQ(conn, fake->conn());
    delete fake;
}

TEST_F(FakeTest, test_fake_connection_send_to_its_session) {

    const std::string kText(expected_text_);
    const std::vector<uint8_t> kBytes(expected_bytes_);
    const auto check_bytes = ElementsAreArray(&expected_bytes_[0], expected_bytes_.size());

    FakeConnection * fake = new FakeConnection();
    Connection::Ptr conn(fake);
    Session::Ptr session(fake->session());

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

    FakeConnection * fake = new FakeConnection();
    Connection::Ptr conn(fake);
    Session::Ptr session(fake->session());

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

    FakeSession * fake = new FakeSession();
    Session::Ptr session(fake);
    Connection::Ptr conn(fake->conn());

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

    FakeSession * fake = new FakeSession();
    Session::Ptr session(fake);
    Connection::Ptr conn(fake->conn());

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
