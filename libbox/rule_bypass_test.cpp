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

    FakeConnection * fake_box(new FakeConnection());
    FakeConnection * fake_user00(new FakeConnection());
    FakeConnection * fake_user01(new FakeConnection());
    FakeConnection * fake_user02(new FakeConnection());

    fake_box->AddRef();
    fake_user00->AddRef();
    fake_user01->AddRef();
    fake_user02->AddRef();

    Session::Ptr s;
    
    s = fake_box->session();
    const std::string kBoxJson(GetBoxJson(kBoxID));
    auto box(BoxProxy::New(s, kBoxJson));

    fake_user00->BindDelegate(&mock);
    s = fake_user00->session();
    auto user00(UserProxy::New(s, GetUserJson("user-00", kBoxID)));

    fake_user01->BindDelegate(&mock);
    s = fake_user01->session();
    auto user01(UserProxy::New(s, GetUserJson("user-01", kBoxID)));

    fake_user02->BindDelegate(&mock);
    s = fake_user02->session();
    auto user02(UserProxy::New(s, GetUserJson("user-02", kBoxID)));

    {
        DummyBypassDelegate d;
        Bypass bypass(box, &d);
        bypass.SetUser(user00);
        bypass.SetUser(user01);
        bypass.SetUser(user02);

        fake_box->SendText("AAA");
    }

    delete fake_user02;
    delete fake_user01;
    delete fake_user00;
    delete fake_box;
}


}  // namespace rule
}  // namespace box
