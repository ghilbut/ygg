#include <gmock/gmock.h>

#include "box_ready.h"
#include "box_ready_delegate.h"
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


class BosProxy;

class BoxServerMock : public BoxReadyDelegate {
public:
    BoxServerMock(BoxReady & box_ready) : box_ready_(box_ready) {}

    MOCK_METHOD1(OnBoxReady, void(BoxProxy*));

private:
    BoxReady & box_ready_;
};


TEST_F(BoxReadyTest, test_set_box_connection) {

    FakeSession session;

    box::BoxReady ready;
    ready.SetBoxSession(&session);
    ASSERT_TRUE(ready.HasBoxSession(&session));
}

TEST_F(BoxReadyTest, test_remove_box_connection_when_disconnected) {

    FakeSession session;

    box::BoxReady ready;
    ready.SetBoxSession(&session);

    session.FireOnClosedEvent();
    ASSERT_FALSE(ready.HasBoxSession(&session));
}


}  // namespace box
