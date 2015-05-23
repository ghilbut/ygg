#include <gmock/gmock.h>

#include "user_info.h"
#include "user_proxy.h"
#include "codebase/session.h"
#include "test/fake.h"


using ::testing::_;
using ::testing::ElementsAreArray;


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

class DelegateMock : public UserProxy::Delegate {
public:
    MOCK_METHOD2(OnText, void(UserProxy*, const std::string&));
    MOCK_METHOD3(OnBinary, void(UserProxy*, const uint8_t[], size_t));
    MOCK_METHOD1(OnClosed, void(UserProxy*));
};

TEST(UserProxyTest, test_delegate) {

    static const std::string kText("text");
    static const uint8_t kBytes[] = "binary";
    static const size_t kBytesSize = sizeof(kBytes) / sizeof(uint8_t);
    
    ASSERT_EQ('b', kBytes[0]);
    ASSERT_EQ('i', kBytes[1]);
    ASSERT_EQ('n', kBytes[2]);
    ASSERT_EQ('a', kBytes[3]);
    ASSERT_EQ('r', kBytes[4]);
    ASSERT_EQ('y', kBytes[5]);

    Session::Ptr session(test::FakeSession::New());
    UserProxy::Ptr proxy(UserProxy::New(session, kValidJson));

    DelegateMock mock;
    EXPECT_CALL(mock, OnText(proxy.get(), kText)).Times(1);
    EXPECT_CALL(mock, OnBinary(proxy.get(), kBytes, kBytesSize)).Times(1);
    EXPECT_CALL(mock, OnClosed(proxy.get())).Times(1);

    proxy->BindDelegate(&mock);
    session->FireOnTextEvent(kText);
    session->FireOnBinaryEvent(kBytes, kBytesSize);
    session->FireOnClosedEvent();
}


}  // namespace box
