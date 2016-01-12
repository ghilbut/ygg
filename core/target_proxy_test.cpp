#include <gmock/gmock.h>
#include "target_proxy.h"
#include "local_connection.h"
#include "test/fake.h"
#include "test/mock.h"
#include "test/vars.h"
#include <json/json.h>


using ::testing::_;
using ::testing::ElementsAreArray;


namespace ygg {


using namespace test;


class TargetProxyTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    id_ = GetRandomString();
    endpoint_ = GetRandomString();
    ctrl_json_ = GetCtrlJson(id_, endpoint_);
    target_json_ = GetTargetJson(endpoint_);
  }

  virtual void TearDown() {
    // nothing
  }

 protected:
  std::string id_;
  std::string endpoint_;
  std::string ctrl_json_;
  std::string target_json_;
};



class MockTargetProxyDelegate : public TargetProxy::Delegate {
 public:
  MOCK_METHOD2(OnText, void(TargetProxy*, const Text&));
  MOCK_METHOD2(OnBinary, void(TargetProxy*, const Bytes&));
  MOCK_METHOD1(OnClosed, void(TargetProxy*));
};



TEST_F(TargetProxyTest, return_object_with_connection_and_desc) {

  Connection::Ptr conn(LocalConnection::New());

  TargetDesc::Ptr desc(TargetDesc::New(target_json_));
  TargetProxy::Ptr proxy(TargetProxy::New(conn, desc));

  ASSERT_NE(nullptr, proxy);
  ASSERT_EQ(target_json_, (proxy->desc()).json);
}

MATCHER_P(IsAck, expected, "is equal to " + expected) {
  Json::Value root;
  Json::Reader reader;
  if (!reader.parse(arg, root)) {
    return false;
  }
  if (!root["endpoint"].isString()) {
    return false;
  }
  return expected == root["endpoint"].asString();
};

TEST_F(TargetProxyTest, ack) {

  auto target(LocalConnection::New());
  auto other(reinterpret_cast<LocalConnection*>(target.get())->other());

  ConnectionDelegateMock mock;
  EXPECT_CALL(mock, OnText(target.get(), IsAck(endpoint_)));
  target->BindDelegate(&mock);

  auto desc(TargetDesc::New(target_json_));
  auto proxy(TargetProxy::New(other, desc));
}

TEST_F(TargetProxyTest, send_text) {

  const auto expected(GetRandomString());

  auto target(LocalConnection::New());
  auto other(reinterpret_cast<LocalConnection*>(target.get())->other());

  auto desc(TargetDesc::New(target_json_));
  auto proxy(TargetProxy::New(other, desc));

  MockTargetProxyDelegate mock;
  EXPECT_CALL(mock, OnText(proxy.get(), expected));
  EXPECT_CALL(mock, OnClosed(proxy.get()));
  proxy->BindDelegate(&mock);

  target->SendText(expected);
  proxy->Close();

  // foo();  // to test for check call stack
}

TEST_F(TargetProxyTest, recv_text) {

  const auto expected(GetRandomString());

  auto target(LocalConnection::New());
  auto other(reinterpret_cast<LocalConnection*>(target.get())->other());

  auto desc(TargetDesc::New(target_json_));
  auto proxy(TargetProxy::New(other, desc));

  ConnectionDelegateMock mock;
  EXPECT_CALL(mock, OnText(target.get(), expected));
  EXPECT_CALL(mock, OnClosed(target.get()));
  target->BindDelegate(&mock);

  proxy->SendText(expected);
  proxy->Close();
}

TEST_F(TargetProxyTest, send_binary) {

  const auto expected(GetRandomBytes());

  auto target(LocalConnection::New());
  auto other(reinterpret_cast<LocalConnection*>(target.get())->other());

  auto desc(TargetDesc::New(target_json_));
  auto proxy(TargetProxy::New(other, desc));

  MockTargetProxyDelegate mock;
  EXPECT_CALL(mock, OnBinary(proxy.get(), expected));
  EXPECT_CALL(mock, OnClosed(proxy.get()));
  proxy->BindDelegate(&mock);

  target->SendBinary(expected);
  proxy->Close();
}

TEST_F(TargetProxyTest, recv_binary) {

  const auto expected(GetRandomBytes());

  auto target(LocalConnection::New());
  auto other(reinterpret_cast<LocalConnection*>(target.get())->other());

  auto desc(TargetDesc::New(target_json_));
  auto proxy(TargetProxy::New(other, desc));

  ConnectionDelegateMock mock;
  EXPECT_CALL(mock, OnBinary(target.get(), expected));
  EXPECT_CALL(mock, OnClosed(target.get()));
  target->BindDelegate(&mock);

  proxy->SendBinary(expected);
  proxy->Close();
}


}  // namesapce ygg
