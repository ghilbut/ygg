#include <gmock/gmock.h>
#include "box_info.h"
#include "box_proxy.h"
#include "net/session.h"


namespace box {


class BoxProxyTest : public ::testing::Test {
};


class FakeSession : public net::Session {
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


TEST_F(BoxProxyTest, test_box_info_returns_null_with_invalid_json_format) {

    const std::string json = "012345678";
    BoxInfo * info = BoxInfo::New(json);

    ASSERT_TRUE(info == nullptr);

    delete info;
}

TEST_F(BoxProxyTest, test_box_info_return_object_with_valid_json_data) {

    const std::string json = "{ \"box-id\": \"box00\" }";
    BoxInfo * info = BoxInfo::New(json);

    ASSERT_TRUE(info != nullptr);
    ASSERT_STREQ("box00", info->id());

    delete info;
}

TEST_F(BoxProxyTest, test_new_box_proxy_returns_null_with_invalid_json_format) {
    const std::string json = "012345678";
    FakeSession session;
    BoxProxy * proxy = BoxProxy::New(json, &session);

    ASSERT_TRUE(proxy == nullptr);

    delete proxy;
}


}  // namespace box
