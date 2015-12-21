#include <gmock/gmock.h>
#include "manager.h"
#include "core/local_connection.h"
#include "test/mock.h"
#include "test/vars.h"
#include <vector>


namespace ygg {
namespace server {
namespace target {


class ManagerTest : public ::testing::Test {
public:
    void SetUp() {

        auto ctrl_conn(LocalConnection::New());
        ctrl_conns_.push_back(ctrl_conn);

        target_conn_ = LocalConnection::New(); 
    }

    void TearDown() {
        target_conn_ = nullptr;
    }

protected:
    std::vector<Connection::Ptr> ctrl_conns_; 
    Connection::Ptr target_conn_; 
};


TEST_F(ManagerTest, ctrls_send_text_to_target) {

    Manager manager;

    auto ctrl_conn = ctrl_conns_[0];
    auto ctrl_conn_tmp = (LocalConnection*) ctrl_conn.get();
    auto ctrl_proxy_conn = ctrl_conn_tmp->other();
    manager.SetCtrlConnection(ctrl_proxy_conn);

    auto target_conn_tmp = (LocalConnection*)target_conn_.get();
    auto target_proxy_conn = target_conn_tmp->other();
    manager.SetTargetConnection(target_proxy_conn);

    target_conn_->SendText(test::GetTargetJson("B"));
    ctrl_conn->SendText(test::GetCtrlJson("A", "B"));

    const auto expected(test::GetRandomString());
    const int n = ctrl_conns_.size();

    test::ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnText(target_conn_.get(), expected)).Times(n);
    target_conn_->BindDelegate(&mock);

    for (auto & ctrl : ctrl_conns_) {
        ctrl->SendText(expected);
    }
}


TEST_F(ManagerTest, ctrls_send_bytes_to_target) {

    Manager manager;

    auto ctrl_conn = ctrl_conns_[0];
    auto ctrl_conn_tmp = (LocalConnection*) ctrl_conn.get();
    auto ctrl_proxy_conn = ctrl_conn_tmp->other();
    manager.SetCtrlConnection(ctrl_proxy_conn);

    auto target_conn_tmp = (LocalConnection*)target_conn_.get();
    auto target_proxy_conn = target_conn_tmp->other();
    manager.SetTargetConnection(target_proxy_conn);

    target_conn_->SendText(test::GetTargetJson("B"));
    ctrl_conn->SendText(test::GetCtrlJson("A", "B"));

    const auto expected(test::GetRandomBytes());
    const int n = ctrl_conns_.size();

    test::ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnBinary(target_conn_.get(), expected)).Times(n);
    target_conn_->BindDelegate(&mock);

    for (auto & ctrl : ctrl_conns_) {
        ctrl->SendBinary(expected);
    }
}


TEST_F(ManagerTest, target_sends_text_to_ctrls) {

    Manager manager;

    auto ctrl_conn = ctrl_conns_[0];
    auto ctrl_conn_tmp = (LocalConnection*) ctrl_conn.get();
    auto ctrl_proxy_conn = ctrl_conn_tmp->other();
    manager.SetCtrlConnection(ctrl_proxy_conn);

    auto target_conn_tmp = (LocalConnection*)target_conn_.get();
    auto target_proxy_conn = target_conn_tmp->other();
    manager.SetTargetConnection(target_proxy_conn);

    target_conn_->SendText(test::GetTargetJson("B"));
    ctrl_conn->SendText(test::GetCtrlJson("A", "B"));

    const auto expected(test::GetRandomString());
    const int n = ctrl_conns_.size();

    test::ConnectionDelegateMock mock;
    for (auto & ctrl : ctrl_conns_) {
        EXPECT_CALL(mock, OnText(ctrl.get(), expected));
        ctrl->BindDelegate(&mock);
    }

    target_conn_->SendText(expected);
}


TEST_F(ManagerTest, target_sends_bytes_to_ctrls) {

    Manager manager;

    auto ctrl_conn = ctrl_conns_[0];
    auto ctrl_conn_tmp = (LocalConnection*) ctrl_conn.get();
    auto ctrl_proxy_conn = ctrl_conn_tmp->other();
    manager.SetCtrlConnection(ctrl_proxy_conn);

    auto target_conn_tmp = (LocalConnection*)target_conn_.get();
    auto target_proxy_conn = target_conn_tmp->other();
    manager.SetTargetConnection(target_proxy_conn);

    target_conn_->SendText(test::GetTargetJson("B"));
    ctrl_conn->SendText(test::GetCtrlJson("A", "B"));

    const auto expected(test::GetRandomBytes());
    const int n = ctrl_conns_.size();

    test::ConnectionDelegateMock mock;
    for (auto & ctrl : ctrl_conns_) {
        EXPECT_CALL(mock, OnBinary(ctrl.get(), expected));
        ctrl->BindDelegate(&mock);
    }

    target_conn_->SendBinary(expected);
}


TEST_F(ManagerTest, target_do_not_fire_onclosed_event_when_ctrls_are_closed) {

    Manager manager;

    auto ctrl_conn = ctrl_conns_[0];
    auto ctrl_conn_tmp = (LocalConnection*) ctrl_conn.get();
    auto ctrl_proxy_conn = ctrl_conn_tmp->other();
    manager.SetCtrlConnection(ctrl_proxy_conn);

    auto target_conn_tmp = (LocalConnection*)target_conn_.get();
    auto target_proxy_conn = target_conn_tmp->other();
    manager.SetTargetConnection(target_proxy_conn);

    target_conn_->SendText(test::GetTargetJson("B"));
    ctrl_conn->SendText(test::GetCtrlJson("A", "B"));

    const auto expected(test::GetRandomBytes());
    const int n = ctrl_conns_.size();

    test::ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnClosed(target_conn_.get())).Times(0);
    target_conn_->BindDelegate(&mock);

    for (auto & ctrl : ctrl_conns_) {
        ctrl->Close();
    }
}


TEST_F(ManagerTest, ctrls_fire_onclosed_event_when_target_is_closed) {

    Manager manager;

    auto ctrl_conn = ctrl_conns_[0];
    auto ctrl_conn_tmp = (LocalConnection*) ctrl_conn.get();
    auto ctrl_proxy_conn = ctrl_conn_tmp->other();
    manager.SetCtrlConnection(ctrl_proxy_conn);

    auto target_conn_tmp = (LocalConnection*)target_conn_.get();
    auto target_proxy_conn = target_conn_tmp->other();
    manager.SetTargetConnection(target_proxy_conn);

    target_conn_->SendText(test::GetTargetJson("B"));
    ctrl_conn->SendText(test::GetCtrlJson("A", "B"));

    const auto expected(test::GetRandomBytes());
    const int n = ctrl_conns_.size();

    test::ConnectionDelegateMock mock;

    for (auto & ctrl : ctrl_conns_) {
        EXPECT_CALL(mock, OnClosed(ctrl.get()));
        ctrl->BindDelegate(&mock);
    }

    target_conn_->Close();
}


TEST_F(ManagerTest, DISABLED_every_connections_fire_onclosed_event_when_clear_manager) {

}


}  // namespace target
}  // namespace server
}  // namespace ygg
