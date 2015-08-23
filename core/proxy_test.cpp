#include <gmock/gmock.h>
#include "test/fake.h"
#include "test_variables.h"
#include "proxy.h"


using ::testing::_;
using ::testing::ElementsAreArray;

using namespace test;



class TestDesc : public codebase::Object<TestDesc> {
public:
    static TestDesc::Ptr New(const std::string & json) {
        return new TestDesc(json);
    }
    ~TestDesc() {}

    const std::string json;

private:
    TestDesc(const std::string & json) 
        : codebase::Object<TestDesc>(this), json(json) {}
};

typedef core::Proxy<TestDesc> TestProxy;



namespace core {


TEST(ProxyTest, test_return_object_with_connection_and_desc) {

    Connection::Ptr conn(FakeConnection::New());
    TestDesc::Ptr desc(TestDesc::New(kCtrlJson));
    typename TestProxy::Ptr proxy(TestProxy::New(conn, desc));

    ASSERT_NE(nullptr, proxy);
    ASSERT_EQ(kCtrlJson, (proxy->desc()).json);
}

class DelegateMock : public TestProxy::Delegate {
public:
    MOCK_METHOD2(OnText, void(TestProxy*, const std::string&));
    MOCK_METHOD2(OnBinary, void(TestProxy*, const std::vector<uint8_t>&));
    MOCK_METHOD1(OnClosed, void(TestProxy*));
};

TEST(ProxyTest, test_delegate) {

    static const uint8_t kExpectedBytes[] = "binary";
    static const size_t kExpectedBytesSize = sizeof(kExpectedBytes) / sizeof(uint8_t);
    ASSERT_EQ('b', kExpectedBytes[0]);
    ASSERT_EQ('i', kExpectedBytes[1]);
    ASSERT_EQ('n', kExpectedBytes[2]);
    ASSERT_EQ('a', kExpectedBytes[3]);
    ASSERT_EQ('r', kExpectedBytes[4]);
    ASSERT_EQ('y', kExpectedBytes[5]);

    static const std::string kText("text");
    static const std::vector<uint8_t> kBytes(kExpectedBytes, kExpectedBytes + kExpectedBytesSize);

    Connection::Ptr conn(FakeConnection::New());
    TestDesc::Ptr desc(TestDesc::New(kCtrlJson));
    TestProxy::Ptr proxy(TestProxy::New(conn, desc));

    DelegateMock mock;
    EXPECT_CALL(mock, OnText(proxy.get(), kText)).Times(1);
    EXPECT_CALL(mock, OnBinary(proxy.get(), ElementsAreArray(kExpectedBytes, kExpectedBytesSize))).Times(1);
    EXPECT_CALL(mock, OnClosed(proxy.get())).Times(1);

    proxy->BindDelegate(&mock);
    conn->FireOnTextEvent(kText);
    conn->FireOnBinaryEvent(kBytes);
    conn->FireOnClosedEvent();
}


}  // namespace core
