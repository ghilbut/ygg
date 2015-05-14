#include <gmock/gmock.h>

#include "user_ready.h"
#include "codebase/ready_delegate.h"
#include "codebase/session.h"


namespace box {


class UserReadyTest : public ::testing::Test {
};


class FakeSession : public codebase::Session {
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


class UserServerMock : public Ready::Delegate {
public:
    UserServerMock(UserReady & ready) : ready_(ready) {}

    MOCK_METHOD1(OnReady, void(Proxy*));

private:
    UserReady & ready_;
};


TEST_F(UserReadyTest, test_set_user_session) {

    FakeSession session;

    UserReady ready;
    ready.SetSession(&session);
    ASSERT_TRUE(ready.HasSession(&session));
}

TEST_F(UserReadyTest, test_remove_user_session_when_disconnected) {

    FakeSession session;

    UserReady ready;
    ready.SetSession(&session);

    session.FireOnClosedEvent();
    ASSERT_FALSE(ready.HasSession(&session));
}


}  // namespace box
