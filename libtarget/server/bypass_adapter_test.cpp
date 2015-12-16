#include <gmock/gmock.h>
#include "bypass_adapter.h"
#include "core/ctrl_desc.h"
#include "core/target_desc.h"
#include "core/proxy.h"
#include "net/local_connection.h"
#include "test/mock.h"
#include "test/vars.h"
#include <string>


namespace ygg {
namespace target {
namespace server {


class BypassAdapterTest : public ::testing::Test {
public:
    void SetUp() {

        ctrl_conn_ = LocalConnection::New();
        auto ctrl_conn_tmp = (LocalConnection*) ctrl_conn_.get();
        auto ctrl_proxy_conn(ctrl_conn_tmp->other());
        const auto ctrl_desc_json(test::GetCtrlJson("A", "B"));
        const auto ctrl_desc(CtrlDesc::New(ctrl_desc_json));
        ctrl_proxy_ = CtrlProxy::New(ctrl_proxy_conn, ctrl_desc);

        target_conn_ = LocalConnection::New();
        auto target_conn_tmp = (LocalConnection*)target_conn_.get();
        auto target_proxy_conn(target_conn_tmp->other());
        const auto target_desc_json(test::GetTargetJson("B"));
        const auto target_desc(TargetDesc::New(target_desc_json));
        target_proxy_ = TargetProxy::New(target_proxy_conn, target_desc);
    }

    void TearDown() {
        target_proxy_ = nullptr;
        target_conn_ = nullptr;
        ctrl_proxy_ = nullptr;
        ctrl_conn_ = nullptr;
    }

protected:
    Connection::Ptr ctrl_conn_;
    CtrlProxy::Ptr ctrl_proxy_;
    Connection::Ptr target_conn_;
    TargetProxy::Ptr target_proxy_;
};


TEST_F(BypassAdapterTest, ctrl_send_text_to_target) {

    auto adapter(BypassAdapter::New(target_proxy_));
    adapter->SetCtrl(ctrl_proxy_);

    const auto expected(test::GetRandomString());

    test::ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnText(target_conn_.get(), expected));
    target_conn_->BindDelegate(&mock);

    ctrl_conn_->SendText(expected);
}


TEST_F(BypassAdapterTest, ctrl_send_bytes_to_target) {

    auto adapter(BypassAdapter::New(target_proxy_));
    adapter->SetCtrl(ctrl_proxy_);

    const auto expected(test::GetRandomBytes());

    test::ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnBinary(target_conn_.get(), expected));
    target_conn_->BindDelegate(&mock);

    ctrl_conn_->SendBinary(expected);
}


TEST_F(BypassAdapterTest, target_send_text_to_ctrl) {

    auto adapter(BypassAdapter::New(target_proxy_));
    adapter->SetCtrl(ctrl_proxy_);

    const auto expected(test::GetRandomString());

    test::ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnText(ctrl_conn_.get(), expected));
    ctrl_conn_->BindDelegate(&mock);

    target_conn_->SendText(expected);
}


TEST_F(BypassAdapterTest, target_send_bytes_to_ctrl) {

    auto adapter(BypassAdapter::New(target_proxy_));
    adapter->SetCtrl(ctrl_proxy_);

    const auto expected(test::GetRandomBytes());

    test::ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnBinary(ctrl_conn_.get(), expected));
    ctrl_conn_->BindDelegate(&mock);

    target_conn_->SendBinary(expected);
}


}  // namespace server
}  // namespace target
}  // namespace ygg

