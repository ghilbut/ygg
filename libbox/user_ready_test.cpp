#include <gmock/gmock.h>

#include "user_ready.h"
#include "user_ready_delegate.h"
#include "net/session.h"


namespace box {


class UserReadyTest : public ::testing::Test {
};


class FakeSession : public net::Session {
public:
    FakeSession() {}
    ~FakeSession() {}

    virtual size_t SendText(const std::string & text) const {
        return text.length();
    }

    virtual size_t SendBinary(const uint8_t bytes[], size_t size) const {
        return size;
    }

    virtual void Close() {
        // nothing
    }

private:
};


class UserProxy;

class UserServerMock : public UserReadyDelegate {
public:
    UserServerMock(UserReady & user_ready) : user_ready_(user_ready) {}

    MOCK_METHOD1(OnUserReady, void(UserProxy*));

private:
    UserReady & user_ready_;
};


TEST_F(UserReadyTest, test_set_user_session) {

    FakeSession session;

    box::UserReady ready;
    ready.SetUserSession(&session);
    ASSERT_TRUE(ready.HasUserSession(&session));
}

TEST_F(UserReadyTest, test_remove_user_session_when_disconnected) {

    FakeSession session;

    box::UserReady ready;
    ready.SetUserSession(&session);

    session.FireOnClosedEvent();
    ASSERT_FALSE(ready.HasUserSession(&session));
}


}  // namespace box
