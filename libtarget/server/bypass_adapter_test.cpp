#include <gmock/gmock.h>
#include "bypass_adapter.h"
#include "core/ctrl_desc.h"
#include "core/target_desc.h"
#include "core/proxy.h"
#include "net/local_connection.h"
#include "test/mock.h"
#include "test/vars.h"
#include <vector>
#include <string>
#include <unordered_map>


using ::testing::_;


namespace ygg {
namespace target {
namespace server {


class NullAdapterDelegate : public BypassAdapter::Delegate {
public:
    virtual void OnClosed(BypassAdapter * adapter) {}
};


class BypassAdapterTest : public ::testing::Test {
public:
    void SetUp() {

        std::vector<std::string> users;
        users.push_back("A0");
        users.push_back("A1");
        users.push_back("A2");

        const std::string endpoint("B");

        for (const auto user : users) {

            auto ctrl_conn(LocalConnection::New());
            ctrl_conns_.push_back(ctrl_conn);

            auto ctrl_conn_tmp = (LocalConnection*) ctrl_conn.get();
            auto ctrl_proxy_conn(ctrl_conn_tmp->other());
            const auto ctrl_desc_json(test::GetCtrlJson(user, endpoint));
            const auto ctrl_desc(CtrlDesc::New(ctrl_desc_json));
            auto ctrl_proxy(CtrlProxy::New(ctrl_proxy_conn, ctrl_desc));
            ctrl_proxys_.push_back(ctrl_proxy);
        }

        target_conn_ = LocalConnection::New();
        auto target_conn_tmp = (LocalConnection*)target_conn_.get();
        auto target_proxy_conn(target_conn_tmp->other());
        const auto target_desc_json(test::GetTargetJson(endpoint));
        const auto target_desc(TargetDesc::New(target_desc_json));
        target_proxy_ = TargetProxy::New(target_proxy_conn, target_desc);
    }

    void TearDown() {
        target_proxy_ = nullptr;
        target_conn_ = nullptr;
        ctrl_proxys_.clear();
        ctrl_conns_.clear();
    }

protected:
    NullAdapterDelegate null_delegate_;

    std::vector<Connection::Ptr> ctrl_conns_;
    std::vector<CtrlProxy::Ptr> ctrl_proxys_;
    Connection::Ptr target_conn_;
    TargetProxy::Ptr target_proxy_;
};


TEST_F(BypassAdapterTest, ctrl_send_text_to_target) {

    auto adapter(BypassAdapter::New(target_proxy_, &null_delegate_));
    for (auto & ctrl : ctrl_proxys_) {
        adapter->SetCtrl(ctrl);
    }

    const auto expected(test::GetRandomString());
    const int n = ctrl_proxys_.size();

    test::ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnText(target_conn_.get(), expected)).Times(n);
    target_conn_->BindDelegate(&mock);

    for (auto & ctrl : ctrl_conns_) {
        ctrl->SendText(expected);
    }
}


TEST_F(BypassAdapterTest, ctrl_send_bytes_to_target) {

    auto adapter(BypassAdapter::New(target_proxy_, &null_delegate_));
    for (auto & ctrl : ctrl_proxys_) {
        adapter->SetCtrl(ctrl);
    }

    const auto expected(test::GetRandomBytes());
    const int n = ctrl_proxys_.size();

    test::ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnBinary(target_conn_.get(), expected)).Times(n);
    target_conn_->BindDelegate(&mock);

    for (auto & ctrl : ctrl_conns_) {
        ctrl->SendBinary(expected);
    }
}


TEST_F(BypassAdapterTest, target_send_text_to_ctrl) {

    auto adapter(BypassAdapter::New(target_proxy_, &null_delegate_));
    for (auto & ctrl : ctrl_proxys_) {
        adapter->SetCtrl(ctrl);
    }

    const auto expected(test::GetRandomString());

    test::ConnectionDelegateMock mock;
    for (auto & ctrl : ctrl_conns_) {
        EXPECT_CALL(mock, OnText(ctrl.get(), expected));
        ctrl->BindDelegate(&mock);
    }

    target_conn_->SendText(expected);
}


TEST_F(BypassAdapterTest, target_send_bytes_to_ctrl) {

    auto adapter(BypassAdapter::New(target_proxy_, &null_delegate_));
    for (auto & ctrl : ctrl_proxys_) {
        adapter->SetCtrl(ctrl);
    }

    const auto expected(test::GetRandomBytes());

    test::ConnectionDelegateMock mock;
    for (auto & ctrl : ctrl_conns_) {
        EXPECT_CALL(mock, OnBinary(ctrl.get(), expected));
        ctrl->BindDelegate(&mock);
    }

    target_conn_->SendBinary(expected);
}


class MockAdapterDelegate : public BypassAdapter::Delegate {
public:
    MOCK_METHOD1(OnClosed, void(BypassAdapter*));
};


TEST_F(BypassAdapterTest, do_not_fire_onclosed_event_when_ctrls_are_closed) {

    MockAdapterDelegate mock;
    EXPECT_CALL(mock, OnClosed(_)).Times(0);

    auto adapter(BypassAdapter::New(target_proxy_, &mock));
    for (auto & ctrl : ctrl_proxys_) {
        adapter->SetCtrl(ctrl);
    }

    for (auto & ctrl : ctrl_proxys_) {
        ctrl->Close();
    }
}


TEST_F(BypassAdapterTest, fire_oncloased_event_when_target_is_closed) {

    MockAdapterDelegate mock;
    EXPECT_CALL(mock, OnClosed(_));

    auto adapter(BypassAdapter::New(target_proxy_, &mock));
    for (auto & ctrl : ctrl_proxys_) {
        adapter->SetCtrl(ctrl);
    }

    target_conn_->Close();
}


}  // namespace server
}  // namespace target
}  // namespace ygg

