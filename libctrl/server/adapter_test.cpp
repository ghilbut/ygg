#include <gmock/gmock.h>

#include "adapter.h"
#include "test/fake.h"
#include "test/mock.h"
#include <json/json.h>


using ::testing::_;

using namespace test;


namespace user {
namespace server {


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

class DummyAdapterDelegate : public Adapter::Delegate {
public:
    virtual void OnClosed(Adapter*) {}
};

TEST(AdapterTest, test_box_client_get_text_when_user_client_send_text) {

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnText(_, "AAA"));

    const char * kBoxID = "box-00";

    Connection::Ptr box_client(FakeConnection::New());
    Connection::Ptr user_client(FakeConnection::New());

    Connection::Ptr server;

    const std::string kBoxJson(GetBoxJson(kBoxID));
    server = ((FakeConnection*) box_client.get())->conn();
    auto box(BoxProxy::New(server, kBoxJson));

    server = ((FakeConnection*) user_client.get())->conn();
    auto user(UserProxy::New(server, GetUserJson("user-00", kBoxID)));

    box_client->BindDelegate(&mock);
    {
        DummyAdapterDelegate d;
        Adapter::Ptr adapter(Adapter::New(user, &d));
        adapter->SetBox(box);

        user_client->SendText("AAA");
    }
}


}  // namespace server
}  // namespace user
