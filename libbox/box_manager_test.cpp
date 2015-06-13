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


TEST_F(BoxManagerTest, test_user_proxy_close_session_when_target_box_is_not_exists) {

    Connection::Ptr conn(FakeConnection::New());
    Session::Ptr session(((FakeConnection*) conn.get())->session());

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnClosed(conn.get()));
    conn->BindDelegate(&mock);

    BoxManager manager;
    manager.BindUserSession(session);

    conn->SendText(valid_user_json());
}

TEST_F(BoxManagerTest, test_bind_user_and_box) {

    static const std::string box_text("box-text");
    static const std::string user_text("user-text");

    // box connection and session
    Connection::Ptr box(FakeConnection::New());
    Session::Ptr box_session(((FakeConnection*) box.get())->session());

    ConnectionDelegateMock box_mock;
    EXPECT_CALL(box_mock, OnText(box.get(), user_text));
    EXPECT_CALL(box_mock, OnClosed(box.get()));
    box->BindDelegate(&box_mock);

    // user connection and session
    Connection::Ptr user(FakeConnection::New());
    Session::Ptr user_session(((FakeConnection*) user.get())->session());

    ConnectionDelegateMock user_mock;
    EXPECT_CALL(user_mock, OnText(user.get(), box_text));
    EXPECT_CALL(user_mock, OnClosed(user.get()));
    user->BindDelegate(&user_mock);

    // bind each other by manager
    BoxManager manager;
    manager.BindBoxSession(box_session);
    manager.BindUserSession(user_session);

    box->SendText(valid_box_json());
    user->SendText(valid_user_json());

    box->SendText(box_text);
    user->SendText(user_text);
}


}  // namespace box
