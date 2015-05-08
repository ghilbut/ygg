#include <gmock/gmock.h>

#include "test/fake_websocket_impl.h"
#include "box_ready.h"
#include "box_ready_delegate.h"


using namespace net::http;
using namespace box;
//using namespace test::fake;


class BoxReadyTest : public ::testing::Test {
};


namespace FakeWebSocket {

class Mock {
public:
    MOCK_CONST_METHOD0(constructed, void());
    MOCK_CONST_METHOD0(destructed, void());
};

class Impl : public WebSocket::Impl {
public:
    explicit Impl(Mock * mock = nullptr) : mock_(mock) {
        if (mock_ != nullptr) {
            mock_->constructed();
        }
    }

    ~Impl() {
        if (mock_ != nullptr) {
            mock_->destructed();
        }
    }

private:
    Mock * mock_;
};

}  // namespace FakeWebSocket



class BosProxy;

class BoxServerMock : public BoxReadyDelegate {
public:
    BoxServerMock(BoxReady & box_ready) : box_ready_(box_ready) {}

    MOCK_METHOD1(OnBoxReady, void(BoxProxy*));

private:
    BoxReady & box_ready_;
};



TEST_F(BoxReadyTest, test_check_lifecycle_of_websocket_impl) {

    // TODO(ghilbut): move to http_websocket_test.cpp

    FakeWebSocket::Mock mock;
    EXPECT_CALL(mock, constructed()).Times(1);
    EXPECT_CALL(mock, destructed()).Times(1);

    FakeWebSocket::Impl * pimpl = new FakeWebSocket::Impl(&mock);
    WebSocket ws(pimpl);
}

TEST_F(BoxReadyTest, test_set_box_connection) {

    FakeWebSocket::Impl * pimpl = new FakeWebSocket::Impl();
    WebSocket ws(pimpl);

    box::BoxReady ready;
    ready.SetBoxConnection(&ws);
    ASSERT_TRUE(ready.HasBoxConnection(&ws));
}

TEST_F(BoxReadyTest, test_remove_box_connection_when_disconnected) {

    FakeWebSocket::Impl * pimpl = new FakeWebSocket::Impl();
    WebSocket ws(pimpl);

    box::BoxReady ready;
    ready.SetBoxConnection(&ws);

    ws.FireOnClosedEvent();
    ASSERT_FALSE(ready.HasBoxConnection(&ws));
}


