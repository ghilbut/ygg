#include <gmock/gmock.h>
#include "ctrl_proxy.h"
#include "local_connection.h"
#include "test/fake.h"
#include "test/mock.h"
#include "test/vars.h"
#include <json/json.h>


using ::testing::_;
using ::testing::ElementsAreArray;


namespace ygg {


using namespace test;


class CtrlProxyTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    user_ = GetRandomString();
    endpoint_ = GetRandomString();
    ctrl_json_ = GetCtrlJson(user_, endpoint_);
    target_json_ = GetTargetJson(endpoint_);
  }

  virtual void TearDown() {
    // nothing
  }

 protected:
  std::string user_;
  std::string endpoint_;
  std::string ctrl_json_;
  std::string target_json_;
};


class MockCtrlProxyDelegate : public CtrlProxy::Delegate {
 public:
  MOCK_METHOD2(OnText, void(CtrlProxy*, const Text&));
  MOCK_METHOD2(OnBinary, void(CtrlProxy*, const Bytes&));
  MOCK_METHOD1(OnClosed, void(CtrlProxy*));
};



TEST_F(CtrlProxyTest, return_object_with_connection_and_desc) {

  Connection::Ptr conn(LocalConnection::New());

  CtrlDesc::Ptr desc(CtrlDesc::New(ctrl_json_));
  CtrlProxy::Ptr proxy(CtrlProxy::New(conn, desc));

  ASSERT_NE(nullptr, proxy);
  ASSERT_EQ(ctrl_json_, (proxy->desc()).json);
}


MATCHER_P2(IsAck, user, endpoint, "is equal to user=" + user + ", endpoint=" +endpoint) {
  Json::Value root;
  Json::Reader reader;
  if (!reader.parse(arg, root)) {
    return false;
  }
  if (!root["user"].isString()) {
    return false;
  }
  if (!root["endpoint"].isString()) {
    return false;
  }
  if (user != root["user"].asString()) {
    return false;
  }
  if (endpoint != root["endpoint"].asString()) {
    return false;
  }
  return true;
};


TEST_F(CtrlProxyTest, ack) {

  auto ctrl(LocalConnection::New());
  auto other(reinterpret_cast<LocalConnection*>(ctrl.get())->other());

  ConnectionDelegateMock mock;
  EXPECT_CALL(mock, OnText(ctrl.get(), IsAck(user_, endpoint_)));
  ctrl->BindDelegate(&mock);

  auto desc(CtrlDesc::New(ctrl_json_));
  auto proxy(CtrlProxy::New(other, desc));
}


TEST_F(CtrlProxyTest, send_text) {

  const auto expected(GetRandomString());

  auto ctrl(LocalConnection::New());
  auto other(reinterpret_cast<LocalConnection*>(ctrl.get())->other());

  auto desc(CtrlDesc::New(ctrl_json_));
  auto proxy(CtrlProxy::New(other, desc));

  MockCtrlProxyDelegate mock;
  EXPECT_CALL(mock, OnText(proxy.get(), expected));
  EXPECT_CALL(mock, OnClosed(proxy.get()));
  proxy->BindDelegate(&mock);

  ctrl->SendText(expected);
  proxy->Close();
}


TEST_F(CtrlProxyTest, recv_text) {

  const auto expected(GetRandomString());

  auto ctrl(LocalConnection::New());
  auto other(reinterpret_cast<LocalConnection*>(ctrl.get())->other());

  auto desc(CtrlDesc::New(ctrl_json_));
  auto proxy(CtrlProxy::New(other, desc));

  ConnectionDelegateMock mock;
  EXPECT_CALL(mock, OnText(ctrl.get(), expected));
  EXPECT_CALL(mock, OnClosed(ctrl.get()));
  ctrl->BindDelegate(&mock);

  proxy->SendText(expected);
  proxy->Close();
}


TEST_F(CtrlProxyTest, send_binary) {

  const auto expected(GetRandomBytes());

  auto ctrl(LocalConnection::New());
  auto other(reinterpret_cast<LocalConnection*>(ctrl.get())->other());

  auto desc(CtrlDesc::New(ctrl_json_));
  auto proxy(CtrlProxy::New(other, desc));

  MockCtrlProxyDelegate mock;
  EXPECT_CALL(mock, OnBinary(proxy.get(), expected));
  EXPECT_CALL(mock, OnClosed(proxy.get()));
  proxy->BindDelegate(&mock);

  ctrl->SendBinary(expected);
  proxy->Close();
}


TEST_F(CtrlProxyTest, recv_binary) {

  const auto expected(GetRandomBytes());

  auto ctrl(LocalConnection::New());
  auto other(reinterpret_cast<LocalConnection*>(ctrl.get())->other());

  auto desc(CtrlDesc::New(ctrl_json_));
  auto proxy(CtrlProxy::New(other, desc));

  ConnectionDelegateMock mock;
  EXPECT_CALL(mock, OnBinary(ctrl.get(), expected));
  EXPECT_CALL(mock, OnClosed(ctrl.get()));
  ctrl->BindDelegate(&mock);

  proxy->SendBinary(expected);
  proxy->Close();
}


}  // namesapce ygg
