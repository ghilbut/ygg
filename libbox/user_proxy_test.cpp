#include <gmock/gmock.h>

#include "user_info.h"
#include "user_proxy.h"
#include "codebase/session.h"
#include "test/test_fake.h"


namespace box {


static const std::string kValidJson = 
    "{ \"user-id\": \"user00\""
    ", \"box-id\": \"box00\" }";

static const std::string kInvalidJson = "012345678";


TEST(UserProxyTest, test_box_info_returns_null_with_invalid_json_format) {

    UserInfo::Ptr info(UserInfo::New(kInvalidJson));
    ASSERT_TRUE(info == nullptr);
}

TEST(UserProxyTest, test_box_info_return_object_with_valid_json_data) {

    UserInfo::Ptr info(UserInfo::New(kValidJson));

    ASSERT_TRUE(info != nullptr);
    ASSERT_STREQ("user00", info->id());
}

TEST(UserProxyTest, test_new_box_proxy_returns_null_with_invalid_json_format) {

    Session::Ptr session(test::FakeSession::New());
    UserProxy::Ptr proxy(UserProxy::New(session, kInvalidJson));

    ASSERT_TRUE(proxy == nullptr);
}

TEST(UserProxyTest, test_new_box_proxy_returns_non_null_with_valid_json_format) {

    Session::Ptr session(test::FakeSession::New());
    UserProxy::Ptr proxy(UserProxy::New(session, kValidJson));

    ASSERT_TRUE(proxy != nullptr);
    ASSERT_STREQ("user00", proxy->info().id());
    ASSERT_STREQ("box00", proxy->box_id());
}


}  // namespace box
