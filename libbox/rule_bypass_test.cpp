#include <gmock/gmock.h>

#include "rule_bypass.h"
#include "test/fake.h"
#include "test/mock.h"
#include <json/json.h>


using ::testing::_;

using namespace test;


namespace box {
namespace rule {


static std::string GetBoxJson(const char * box_id) {

    Json::Value root(Json::objectValue);
    root["box-id"] = box_id;
    
    Json::FastWriter writer;
    return writer.write(root);
}

static std::string GetUserJson(const char * user_id, const char * box_id) {
    Json::Value root(Json::objectValue);
    root["user-id"] = user_id;
    root["box-id"] = box_id;
    
    Json::FastWriter writer;
    return writer.write(root);
}

class DummyBypassDelegate : public Bypass::Delegate {
public:
    virtual void OnClosed(Bypass *) {}
};

TEST(RuleBypassTest, test_user_sessions_get_text_when_box_session_send_text) {

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnText(_, "AAA")).Times(3);

    const char * kBoxID = "box-00";

    Connection::Ptr box_conn(FakeConnection::New());
    Connection::Ptr user_conn_00(FakeConnection::New());
    Connection::Ptr user_conn_01(FakeConnection::New());
    Connection::Ptr user_conn_02(FakeConnection::New());

    Session::Ptr s;
    
    s = ((FakeConnection*) box_conn.get())->session();
    const std::string kBoxJson(GetBoxJson(kBoxID));
    auto box(BoxProxy::New(s, kBoxJson));

    user_conn_00->BindDelegate(&mock);
    s = ((FakeConnection*) user_conn_00.get())->session();
    auto user00(UserProxy::New(s, GetUserJson("user-00", kBoxID)));

    user_conn_01->BindDelegate(&mock);
    s = ((FakeConnection*) user_conn_01.get())->session();
    auto user01(UserProxy::New(s, GetUserJson("user-01", kBoxID)));

    user_conn_02->BindDelegate(&mock);
    s = ((FakeConnection*) user_conn_02.get())->session();
    auto user02(UserProxy::New(s, GetUserJson("user-02", kBoxID)));

    {
        DummyBypassDelegate d;
        Bypass::Ptr bypass(Bypass::New(box, &d));
        bypass->SetUser(user00);
        bypass->SetUser(user01);
        bypass->SetUser(user02);

        box_conn->SendText("AAA");
    }
}


}  // namespace rule
}  // namespace box