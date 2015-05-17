#include <gmock/gmock.h>
#include "user_info.h"
#include "user_proxy.h"
#include "codebase/session.h"


namespace box {


class UserProxyTest : public ::testing::Test {
};


class FakeSession : public codebase::Session {
public:
    virtual size_t SendText(const std::string & text) const {
        return text.length();
    }

    virtual size_t SendBinary(const uint8_t bytes[], size_t size) const {
        return size;
    }

    virtual void Close() {
    }
};


TEST_F(UserProxyTest, test_box_info_returns_null_with_invalid_json_format) {

    const std::string json = "012345678";
    UserInfo::Ptr info(UserInfo::New(json));

    ASSERT_TRUE(info == nullptr);
}

TEST_F(UserProxyTest, test_box_info_return_object_with_valid_json_data) {

    const std::string json = 
        "{ \"user-id\": \"user00\""
        ", \"box-id\": \"box00\" }";

    UserInfo::Ptr info(UserInfo::New(json));

    ASSERT_TRUE(info != nullptr);
    ASSERT_STREQ("user00", info->id());
    //ASSERT_STREQ("box00", info->box_id());
}

TEST_F(UserProxyTest, test_new_box_proxy_returns_null_with_invalid_json_format) {
    const std::string json = "012345678";
    FakeSession session;
    UserProxy * proxy = UserProxy::New(&session, json);

    ASSERT_TRUE(proxy == nullptr);

    delete proxy;
}


}  // namespace box
