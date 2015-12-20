#include <gmock/gmock.h>
#include "local_connection.h"
#include "proxy.h"
#include "test/fake.h"
#include "test/mock.h"
#include "test/vars.h"


using ::testing::_;
using ::testing::ElementsAreArray;

using namespace ygg::test;


namespace ygg {


static const std::string kCtrlJson(GetCtrlJson("A", "B"));
static const std::string kTargetJson(GetTargetJson("B"));


class TestDesc : public Object {
public:
    typedef boost::intrusive_ptr<TestDesc> Ptr;

    static TestDesc::Ptr New(const std::string & json) {
        return new TestDesc(json);
    }
    ~TestDesc() {}

    const std::string json;

private:
    TestDesc(const std::string & json) 
        : Object(), json(json) {}
};

typedef Proxy<TestDesc> TestProxy;



class ProxyTest : public ::testing::Test {
public:
    ProxyTest() {}
    ~ProxyTest() {}

    virtual void SetUp() {

        srand((unsigned int) time(nullptr));

        static const std::string kAlphaDigit(
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789");

        const int size = 100 + (rand() % 100);
        for (int i = 0; i < size; ++i) {
            const int r = rand() % kAlphaDigit.length();
            const char c = kAlphaDigit[r-1];
            text_.push_back(c);
            bytes_.push_back(c);
        }
    }

    virtual void TearDown() {
        // nothing
    }

public:
    std::string text_;
    std::vector<uint8_t> bytes_;
};



class DelegateMock : public TestProxy::Delegate {
public:
    MOCK_METHOD2(OnText, void(TestProxy*, const Text&));
    MOCK_METHOD2(OnBinary, void(TestProxy*, const Bytes&));
    MOCK_METHOD1(OnClosed, void(TestProxy*));
};



TEST_F(ProxyTest, return_object_with_connection_and_desc) {

    Connection::Ptr conn(FakeConnection::New());
    TestDesc::Ptr desc(TestDesc::New(kCtrlJson));
    typename TestProxy::Ptr proxy(TestProxy::New(conn, desc));

    ASSERT_NE(nullptr, proxy);
    ASSERT_EQ(kCtrlJson, (proxy->desc()).json);
}

TEST_F(ProxyTest, send_text) {

    const Text kExpected(text_);

    Connection::Ptr user_conn(LocalConnection::New());
    Connection::Ptr proxy_conn(((LocalConnection*)user_conn.get())->other());

    TestDesc::Ptr desc(TestDesc::New(kCtrlJson));
    TestProxy::Ptr proxy(TestProxy::New(proxy_conn, desc));

    DelegateMock mock;
    EXPECT_CALL(mock, OnText(proxy.get(), kExpected));
    EXPECT_CALL(mock, OnClosed(proxy.get()));
    proxy->BindDelegate(&mock);

    user_conn->SendText(text_);
    proxy->Close();

    // foo();    // to test for check call stack
}

TEST_F(ProxyTest, recv_text) {

    const Text kExpected(text_);

    Connection::Ptr user_conn(LocalConnection::New());
    Connection::Ptr proxy_conn(((LocalConnection*)user_conn.get())->other());

    TestDesc::Ptr desc(TestDesc::New(kCtrlJson));
    TestProxy::Ptr proxy(TestProxy::New(proxy_conn, desc));

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnText(user_conn.get(), kExpected));
    EXPECT_CALL(mock, OnClosed(user_conn.get()));
    user_conn->BindDelegate(&mock);

    proxy->SendText(text_);
    proxy->Close();
}

TEST_F(ProxyTest, send_binary) {

    const Bytes kExpected(bytes_);

    Connection::Ptr user_conn(LocalConnection::New());
    Connection::Ptr proxy_conn(((LocalConnection*)user_conn.get())->other());

    TestDesc::Ptr desc(TestDesc::New(kCtrlJson));
    TestProxy::Ptr proxy(TestProxy::New(proxy_conn, desc));

    DelegateMock mock;
    EXPECT_CALL(mock, OnBinary(proxy.get(), kExpected));
    EXPECT_CALL(mock, OnClosed(proxy.get()));
    proxy->BindDelegate(&mock);

    user_conn->SendBinary(bytes_);
    proxy->Close();
}

TEST_F(ProxyTest, recv_binary) {

    const Bytes kExpected(bytes_);

    Connection::Ptr user_conn(LocalConnection::New());
    Connection::Ptr proxy_conn(((LocalConnection*)user_conn.get())->other());

    TestDesc::Ptr desc(TestDesc::New(kCtrlJson));
    TestProxy::Ptr proxy(TestProxy::New(proxy_conn, desc));

    ConnectionDelegateMock mock;
    EXPECT_CALL(mock, OnBinary(user_conn.get(), kExpected));
    EXPECT_CALL(mock, OnClosed(user_conn.get()));
    user_conn->BindDelegate(&mock);

    proxy->SendBinary(bytes_);
    proxy->Close();
}


}  // namesapce ygg
