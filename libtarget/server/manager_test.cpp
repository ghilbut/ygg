#include <gmock/gmock.h>
#include "manager.h"
#include "net/local_connection.h"
#include "test/mock.h"
#include "test/vars.h"
#include <vector>


namespace ygg {
namespace target {
namespace server {


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


TEST_F(ManagerTest, test) {

    Manager manager;

    auto ctrl_conn = ctrl_conns_[0];
    auto ctrl_conn_tmp = (LocalConnection*) ctrl_conn.get();

    auto target_conn_tmp = (LocalConnection*)target_conn_.get();

    auto a = ctrl_conn_tmp->other();
    manager.SetCtrlConnection(a);
    auto b = target_conn_tmp->other();
    manager.SetTargetConnection(b);

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


}  // namespace ygg
}  // namespace target
}  // namespace server
