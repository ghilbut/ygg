#include <gmock/gmock.h>

#include "box_info.h"
#include "box_proxy.h"
#include "codebase/session.h"
#include "test/fake.h"


namespace box {


static const std::string kValidJson = "{ \"box-id\": \"box00\" }";

static const std::string kInvalidJson = "012345678";


TEST(BoxProxyTest, test_box_info_returns_null_with_invalid_json_format) {

    BoxInfo::Ptr info(BoxInfo::New(kInvalidJson));

    ASSERT_TRUE(info == nullptr);
}

TEST(BoxProxyTest, test_box_info_return_object_with_valid_json_data) {

    BoxInfo::Ptr info(BoxInfo::New(kValidJson));

    ASSERT_TRUE(info != nullptr);
    ASSERT_STREQ("box00", info->id());
}

TEST(BoxProxyTest, test_new_box_proxy_returns_null_with_invalid_json_format) {

    Session::Ptr session(test::FakeSession::New());
    BoxProxy::Ptr proxy(BoxProxy::New(session, kInvalidJson));

    ASSERT_TRUE(proxy == nullptr);
}

TEST(BoxProxyTest, test_new_box_proxy_returns_non_null_with_valid_json_format) {

    Session::Ptr session(test::FakeSession::New());
    BoxProxy::Ptr proxy(BoxProxy::New(session, kValidJson));

    ASSERT_TRUE(proxy != nullptr);
    ASSERT_STREQ("box00", proxy->info().id());
}


}  // namespace box
