#include <gmock/gmock.h>

#include "box_ready.h"
#include "ready_delegate.h"
#include "net/session.h"


namespace box {


class BoxReadyTest : public ::testing::Test {
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


class BoxServerMock : public Ready::Delegate {
public:
    BoxServerMock(BoxReady & ready) : ready_(ready) {}

    MOCK_METHOD1(OnReady, void(Proxy*));

private:
    BoxReady & ready_;
};


TEST_F(BoxReadyTest, test_set_box_session) {

    FakeSession session;

    box::BoxReady ready;
    ready.SetSession(&session);
    ASSERT_TRUE(ready.HasSession(&session));
}

TEST_F(BoxReadyTest, test_remove_box_session_when_disconnected) {

    FakeSession session;

    box::BoxReady ready;
    ready.SetSession(&session);

    session.FireOnClosedEvent();
    ASSERT_FALSE(ready.HasSession(&session));
}


}  // namespace box
