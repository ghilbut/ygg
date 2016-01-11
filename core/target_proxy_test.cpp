#include <gmock/gmock.h>
#include "target_proxy.h"
#include "local_connection.h"
#include "test/fake.h"
#include "test/mock.h"
#include "test/vars.h"


using ::testing::_;
using ::testing::ElementsAreArray;

using namespace ygg::test;


namespace ygg {


static const std::string kCtrlJson(GetCtrlJson("A", "B"));
static const std::string kTargetJson(GetTargetJson("B"));


class TargetProxyTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
  text_ = GetRandomString();
  bytes_ = GetRandomBytes();
  }

  virtual void TearDown() {
  }

 protected:
  std::string text_;
  std::vector<uint8_t> bytes_;
};



class DelegateMock : public TargetProxy::Delegate {
 public:
  MOCK_METHOD2(OnText, void(TargetProxy*, const Text&));
  MOCK_METHOD2(OnBinary, void(TargetProxy*, const Bytes&));
  MOCK_METHOD1(OnClosed, void(TargetProxy*));
};



TEST_F(TargetProxyTest, return_object_with_connection_and_desc) {

//  Connection::Ptr conn(FakeConnection::New());
  Connection::Ptr conn(LocalConnection::New());

  ConnectionDelegateMock conn_mock;
  conn->BindDelegate(&conn_mock);

  TargetDesc::Ptr desc(TargetDesc::New(kCtrlJson));
  TargetProxy::Ptr proxy(TargetProxy::New(conn, desc));

  ASSERT_NE(nullptr, proxy);
  ASSERT_EQ(kCtrlJson, (proxy->desc()).json);

  conn->UnbindDelegate();
}

TEST_F(TargetProxyTest, send_text) {

  const Text kExpected(text_);

  Connection::Ptr user_conn(LocalConnection::New());
  Connection::Ptr proxy_conn(((LocalConnection*)user_conn.get())->other());

  TargetDesc::Ptr desc(TargetDesc::New(kCtrlJson));
  TargetProxy::Ptr proxy(TargetProxy::New(proxy_conn, desc));

  DelegateMock mock;
  EXPECT_CALL(mock, OnText(proxy.get(), kExpected));
  EXPECT_CALL(mock, OnClosed(proxy.get()));
  proxy->BindDelegate(&mock);

  user_conn->SendText(text_);
  proxy->Close();

  // foo();  // to test for check call stack
}

TEST_F(TargetProxyTest, recv_text) {

  const Text kExpected(text_);

  Connection::Ptr user_conn(LocalConnection::New());
  Connection::Ptr proxy_conn(((LocalConnection*)user_conn.get())->other());

  TargetDesc::Ptr desc(TargetDesc::New(kCtrlJson));
  TargetProxy::Ptr proxy(TargetProxy::New(proxy_conn, desc));

  ConnectionDelegateMock mock;
  EXPECT_CALL(mock, OnText(user_conn.get(), kExpected));
  EXPECT_CALL(mock, OnClosed(user_conn.get()));
  user_conn->BindDelegate(&mock);

  proxy->SendText(text_);
  proxy->Close();
}

TEST_F(TargetProxyTest, send_binary) {

  const Bytes kExpected(bytes_);

  Connection::Ptr user_conn(LocalConnection::New());
  Connection::Ptr proxy_conn(((LocalConnection*)user_conn.get())->other());

  TargetDesc::Ptr desc(TargetDesc::New(kCtrlJson));
  TargetProxy::Ptr proxy(TargetProxy::New(proxy_conn, desc));

  DelegateMock mock;
  EXPECT_CALL(mock, OnBinary(proxy.get(), kExpected));
  EXPECT_CALL(mock, OnClosed(proxy.get()));
  proxy->BindDelegate(&mock);

  user_conn->SendBinary(bytes_);
  proxy->Close();
}

TEST_F(TargetProxyTest, recv_binary) {

  const Bytes kExpected(bytes_);

  Connection::Ptr user_conn(LocalConnection::New());
  Connection::Ptr proxy_conn(((LocalConnection*)user_conn.get())->other());

  TargetDesc::Ptr desc(TargetDesc::New(kCtrlJson));
  TargetProxy::Ptr proxy(TargetProxy::New(proxy_conn, desc));

  ConnectionDelegateMock mock;
  EXPECT_CALL(mock, OnBinary(user_conn.get(), kExpected));
  EXPECT_CALL(mock, OnClosed(user_conn.get()));
  user_conn->BindDelegate(&mock);

  proxy->SendBinary(bytes_);
  proxy->Close();
}


}  // namesapce ygg
