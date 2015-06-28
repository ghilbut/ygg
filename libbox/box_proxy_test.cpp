#include <gmock/gmock.h>

#include "box_proxy.h"
#include "codebase/box_desc.h"
#include "codebase/connection.h"
#include "test/fake.h"


using ::testing::_;

using namespace test;


namespace box {


static const std::string kValidJson = "{ \"box-id\": \"box00\" }";

static const std::string kInvalidJson = "012345678";


TEST(BoxProxyTest, test_box_info_returns_null_with_invalid_json_format) {

    BoxDesc::Ptr info(BoxDesc::New(kInvalidJson));

    ASSERT_TRUE(info == nullptr);
}

TEST(BoxProxyTest, test_box_info_return_object_with_valid_json_data) {

    BoxDesc::Ptr info(BoxDesc::New(kValidJson));

    ASSERT_TRUE(info != nullptr);
    ASSERT_STREQ("box00", info->id());
}

TEST(BoxProxyTest, test_new_box_proxy_returns_null_with_invalid_json_format) {

    Connection::Ptr conn(FakeConnection::New());
    BoxProxy::Ptr proxy(BoxProxy::New(conn, kInvalidJson));

    ASSERT_TRUE(proxy == nullptr);
}

TEST(BoxProxyTest, test_new_box_proxy_returns_non_null_with_valid_json_format) {

    Connection::Ptr conn(FakeConnection::New());
    BoxProxy::Ptr proxy(BoxProxy::New(conn, kValidJson));

    ASSERT_TRUE(proxy != nullptr);
    ASSERT_STREQ("box00", proxy->info().id());
}

class DelegateMock : public BoxProxy::Delegate {
public:
    MOCK_METHOD2(OnText, void(BoxProxy*, const std::string&));
    MOCK_METHOD2(OnBinary, void(BoxProxy*, const std::vector<uint8_t>&));
    MOCK_METHOD1(OnClosed, void(BoxProxy*));
};

TEST(BoxProxyTest, test_delegate) {

    static const uint8_t kExpectedBytes[] = "binary";
    static const size_t kExpectedBytesSize = sizeof(kExpectedBytes) / sizeof(uint8_t);

    static const std::string kText("text");
    static const std::vector<uint8_t> kBytes(kExpectedBytes, kExpectedBytes + kExpectedBytesSize);

    Connection::Ptr conn(FakeConnection::New());
    BoxProxy::Ptr proxy(BoxProxy::New(conn, kValidJson));

    DelegateMock mock;
    BoxProxy * p = proxy.get();
    EXPECT_CALL(mock, OnText(p, _)).Times(1);
    EXPECT_CALL(mock, OnBinary(p, _)).Times(1);
    EXPECT_CALL(mock, OnClosed(p)).Times(1);

    proxy->BindDelegate(&mock);
    conn->FireOnTextEvent(kText);
    conn->FireOnBinaryEvent(kBytes);
    conn->FireOnClosedEvent();
}


}  // namespace box
