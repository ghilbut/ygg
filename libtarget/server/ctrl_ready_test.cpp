#include <gmock/gmock.h>
#include "ctrl_ready.h"
#include "net/local_connection.h"
#include "test/mock.h"
#include <json/json.h>
#include <string>

using namespace ygg::core;
using namespace ygg::net;

using ::testing::_;


namespace ygg {
namespace target {
namespace server {


static std::string GetCtrlJson() {

    Json::Value root(Json::objectValue);
    root["user"] = "A";
    root["endpoint"] = "B";

    Json::FastWriter w;
    return w.write(root);
}


class CtrlReadyTest : public ::testing::Test {
public:
    CtrlReadyTest()
        : kCtrlJson_(GetCtrlJson()) {
        // nothing
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

protected:
    const std::string kCtrlJson_;
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

    test::MockCtrlReadyDelegate mock;
    EXPECT_CALL(mock, OnProxy(_));

    CtrlReady ready(&mock);
    ready.SetConnection(proxy_conn);
    ASSERT_TRUE(ready.HasConnection(proxy_conn));

    ctrl_conn->SendText(kCtrlJson_);
    ASSERT_FALSE(ready.HasConnection(proxy_conn));
}


}  // namespace server
}  // namespace target
}  // namespace ygg
