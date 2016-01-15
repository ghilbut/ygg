#include <gmock/gmock.h>
#include "target_ready.h"
#include "core/local_connection.h"
#include "core/test/mock_connection_delegate.h"
#include "test/mock.h"
#include "test/vars.h"
#include <json/json.h>
#include <string>


using ::testing::_;


namespace ygg {
namespace server {
namespace target {


class TargetReadyTest : public ::testing::Test {
public:
    TargetReadyTest()
        : kTargetJson_(test::GetTargetJson("B")) {
        // nothing
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

protected:
    const std::string kTargetJson_;
};


class MockTargetReadyDelegate : public TargetReady::Delegate {
 public:
  MOCK_METHOD1(OnProxy, void (TargetProxy::Ptr&));
};


class NullTargetReadyDelegate : public TargetReady::Delegate {
public:
    void OnProxy(TargetProxy::Ptr&) {};
};


TEST_F(TargetReadyTest, close_connection_when_invalid_json_is_injected) {

    Connection::Ptr target_conn(LocalConnection::New());
    Connection::Ptr proxy_conn(((LocalConnection*)target_conn.get())->other());

    test::MockConnectionDelegate mock;
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

    MockTargetReadyDelegate mock;
    EXPECT_CALL(mock, OnProxy(_));

    TargetReady ready(&mock);
    ready.SetConnection(proxy_conn);
    ASSERT_TRUE(ready.HasConnection(proxy_conn));

    target_conn->SendText(kTargetJson_);
    ASSERT_FALSE(ready.HasConnection(proxy_conn));
}


}  // namespace target
}  // namespace server
}  // namespace ygg
