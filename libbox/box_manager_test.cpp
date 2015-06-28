#include <gmock/gmock.h>

#include "box_manager.h"
#include "test/fake.h"
#include "test/mock.h"
#include <json/json.h>


using ::testing::_;

using namespace test;


namespace box {


class BoxManagerTest : public ::testing::Test {
public:
    BoxManagerTest() {

        Json::FastWriter writer;

        Json::Value box(Json::objectValue);
        box["box-id"] = "box00";
        valid_box_json_ = writer.write(box);

        Json::Value user(Json::objectValue);
        user["user-id"] = "user00";
        user["box-id"] = "box00";
        valid_user_json_ = writer.write(user);
    }

    ~BoxManagerTest() {}

    virtual void SetUp() {}
    virtual void TearDown() {}

    const std::string & valid_box_json() const {
        return valid_box_json_;
    }

    const std::string & valid_user_json() const {
        return valid_user_json_;
    }

private:
    std::string invalid_box_json_;
    std::string valid_box_json_;
    std::string invalid_user_json_;
    std::string valid_user_json_;
};


TEST_F(BoxManagerTest, test_user_proxy_close_conn_when_target_box_is_not_exists) {

    Connection::Ptr client(FakeConnection::New());
    Connection::Ptr server(((FakeConnection*) client.get())->conn());

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnClosed(client.get()));
    client->BindDelegate(&mock);

    BoxManager manager;
    manager.BindUserConnection(server);

    client->SendText(valid_user_json());
}

TEST_F(BoxManagerTest, test_bind_user_and_box) {

    static const std::string box_text("box-text");
    static const std::string user_text("user-text");

    // box connection and conn
    Connection::Ptr box(FakeConnection::New());
    Connection::Ptr box_conn(((FakeConnection*) box.get())->conn());

    ConnectionDelegateMock box_mock;
    EXPECT_CALL(box_mock, OnText(box.get(), user_text));
    EXPECT_CALL(box_mock, OnClosed(box.get()));
    box->BindDelegate(&box_mock);

    // user connection and conn
    Connection::Ptr user(FakeConnection::New());
    Connection::Ptr user_conn(((FakeConnection*) user.get())->conn());

    ConnectionDelegateMock user_mock;
    EXPECT_CALL(user_mock, OnText(user.get(), box_text));
    EXPECT_CALL(user_mock, OnClosed(user.get()));
    user->BindDelegate(&user_mock);

    // bind each other by manager
    BoxManager manager;
    manager.BindBoxConnection(box_conn);
    manager.BindUserConnection(user_conn);

    box->SendText(valid_box_json());
    user->SendText(valid_user_json());

    box->SendText(box_text);
    user->SendText(user_text);
}


}  // namespace box
