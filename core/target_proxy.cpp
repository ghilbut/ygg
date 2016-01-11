#include "target_proxy.h"
#include <json/json.h>


namespace ygg {


class NullDelegate : public TargetProxy::Delegate {
 public:
  virtual void OnText(TargetProxy*, const Text&) {}
  virtual void OnBinary(TargetProxy*, const Bytes&) {}
  virtual void OnClosed(TargetProxy*) {}
};

static NullDelegate kNullDelegate;


TargetProxy::Ptr TargetProxy::New(Connection::Ptr & conn,
                                  const TargetDesc::Ptr & desc) {
  assert(conn != nullptr);
  assert(desc != nullptr);
  return new TargetProxy(conn, desc);
}

TargetProxy::~TargetProxy() {
  // NOTE(ghilbut): below code makes 
  // "libc++abi.dylib: Pure virtual function called!" runtime error
  // because calling Close() function of Connection interface
  // Close();
}

size_t TargetProxy::SendText(const std::string & text) const {
  assert(conn_ != nullptr);
  return conn_->SendText(text);
}

size_t TargetProxy::SendBinary(const std::vector<uint8_t> & bytes) const {
  assert(conn_ != nullptr);
  return conn_->SendBinary(bytes);
}

void TargetProxy::Close() {
  assert(conn_ != nullptr);
  conn_->Close();
}

void TargetProxy::BindDelegate(Delegate * delegate) {
  assert(delegate != nullptr);
  delegate_ = delegate;
}

void TargetProxy::UnbindDelegate() {
  delegate_ = &kNullDelegate;
}

const TargetDesc & TargetProxy::desc() const {
  return *desc_;
}

void TargetProxy::OnText(Connection * conn, const Text & text) {
  assert(conn != nullptr);
  assert(conn == conn_);
  delegate_->OnText(this, text);
}

void TargetProxy::OnBinary(Connection * conn, const Bytes & bytes) {
  assert(conn != nullptr);
  assert(conn == conn_);
  delegate_->OnBinary(this, bytes);
}

void TargetProxy::OnClosed(Connection * conn) {
  assert(conn != nullptr);
  assert(conn == conn_);
  delegate_->OnClosed(this);
  conn_->UnbindDelegate();
}

TargetProxy::TargetProxy(Connection::Ptr & conn,
                         const TargetDesc::Ptr & desc)
  : Object()
  , delegate_(&kNullDelegate)
  , conn_(conn)
  , desc_(desc) {

  assert(conn_ != nullptr);
  assert(desc_ != nullptr);
  conn_->BindDelegate(this);

  Json::Value root(Json::objectValue);
  root["endpoint"] = desc_->endpoint;
  Json::FastWriter writer;
  const std::string ack = writer.write(root);
  conn_->SendText(ack);
}


}  // namespace ygg
