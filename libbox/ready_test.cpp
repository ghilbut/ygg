#include <gmock/gmock.h>

#include "ready_delegate.h"
#include "net/session.h"


namespace box {


class ReadyTest : public ::testing::Test {
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


class ServerMock : public Ready::Delegate {
public:
    ServerMock(Ready & ready) : ready_(ready) {}

    MOCK_METHOD1(OnReady, void(Proxy*));

private:
    Ready & ready_;
};


TEST_F(ReadyTest, test_set_session) {

    FakeSession session;

    box::Ready ready;
    ready.SetSession(&session);
    ASSERT_TRUE(ready.HasSession(&session));
}

TEST_F(ReadyTest, test_remove_session_when_disconnected) {

    FakeSession session;

    box::Ready ready;
    ready.SetSession(&session);

    session.FireOnClosedEvent();
    ASSERT_FALSE(ready.HasSession(&session));
}


}  // namespace box
