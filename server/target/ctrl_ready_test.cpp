#include <gmock/gmock.h>
#include "ctrl_ready.h"
#include "core/local_connection.h"
#include "test/mock.h"
#include "test/vars.h"
#include <json/json.h>
#include <string>


using ::testing::_;


namespace ygg {
namespace server {
namespace target {


class CtrlReadyTest : public ::testing::Test {
public:
    CtrlReadyTest()
        : kCtrlJson_(test::GetCtrlJson("A", "B")) {
        // nothing
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

protected:
    const std::string kCtrlJson_;
};


class MockCtrlReadyDelegate : public CtrlReady::Delegate {
 public:
  MOCK_METHOD1(OnProxy, void (CtrlProxy::Ptr&));
};


class NullCtrlReadyDelegate : public CtrlReady::Delegate {
public:
    void OnProxy(CtrlProxy::Ptr&) {};
};


TEST_F(CtrlReadyTest, close_connection_when_invalid_json_is_injected) {

    Connection::Ptr ctrl_conn(LocalConnection::New());
    Connection::Ptr proxy_conn(((LocalConnection*)ctrl_conn.get())->other());

    test::ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnClosed(ctrl_conn.get()));
    ctrl_conn->BindDelegate(&mock);

    NullCtrlReadyDelegate null_delegate;
    CtrlReady ready(&null_delegate);

    ready.SetConnection(proxy_conn);
    ASSERT_TRUE(ready.HasConnection(proxy_conn));

    ctrl_conn->SendText("");
    ASSERT_FALSE(ready.HasConnection(proxy_conn));
}


TEST_F(CtrlReadyTest, pass_proxy_when_get_valid_json_from_connection) {

    Connection::Ptr ctrl_conn(LocalConnection::New());
    Connection::Ptr proxy_conn(((LocalConnection*)ctrl_conn.get())->other());

    MockCtrlReadyDelegate mock;
    EXPECT_CALL(mock, OnProxy(_));

    CtrlReady ready(&mock);
    ready.SetConnection(proxy_conn);
    ASSERT_TRUE(ready.HasConnection(proxy_conn));

    ctrl_conn->SendText(kCtrlJson_);
    ASSERT_FALSE(ready.HasConnection(proxy_conn));
}


}  // namespace target
}  // namespace server
}  // namespace ygg
