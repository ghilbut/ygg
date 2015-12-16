#include <gmock/gmock.h>
#include "target_ready.h"
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


static std::string GetTargetJson() {

    Json::Value root(Json::objectValue);
    root["endpoint"] = "B";

    Json::FastWriter w;
    return w.write(root);
}


class TargetReadyTest : public ::testing::Test {
public:
    TargetReadyTest()
        : kTargetJson_(GetTargetJson()) {
        // nothing
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

protected:
    const std::string kTargetJson_;
};


class NullTargetReadyDelegate : public TargetReady::Delegate {
public:
    void OnProxy(TargetProxy::Ptr&) {};
};


TEST_F(TargetReadyTest, close_connection_when_invalid_json_is_injected) {

    Connection::Ptr target_conn(LocalConnection::New());
    Connection::Ptr proxy_conn(((LocalConnection*)target_conn.get())->other());

    test::ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnClosed(target_conn.get()));
    target_conn->BindDelegate(&mock);

    NullTargetReadyDelegate null_delegate;
    TargetReady ready(&null_delegate);

    ready.SetConnection(proxy_conn);
    ASSERT_TRUE(ready.HasConnection(proxy_conn));

    target_conn->SendText("");
    ASSERT_FALSE(ready.HasConnection(proxy_conn));
}


TEST_F(TargetReadyTest, pass_proxy_when_get_valid_json_from_connection) {

    Connection::Ptr target_conn(LocalConnection::New());
    Connection::Ptr proxy_conn(((LocalConnection*)target_conn.get())->other());

    test::MockTargetReadyDelegate mock;
    EXPECT_CALL(mock, OnProxy(_));

    TargetReady ready(&mock);
    ready.SetConnection(proxy_conn);
    ASSERT_TRUE(ready.HasConnection(proxy_conn));

    target_conn->SendText(kTargetJson_);
    ASSERT_FALSE(ready.HasConnection(proxy_conn));
}


}  // namespace server
}  // namespace target
}  // namespace ygg
