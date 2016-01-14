#include "ctrl_proxy.h"
#include <json/json.h>


namespace ygg {


class NullDelegate : public CtrlProxy::Delegate {
 public:
  virtual void OnText(CtrlProxy*, const Text&) {}
  virtual void OnBinary(CtrlProxy*, const Bytes&) {}
  virtual void OnClosed(CtrlProxy*) {}
};

static NullDelegate kNullDelegate;


CtrlProxy::Ptr CtrlProxy::New(Connection::Ptr & conn,
                              const CtrlDesc::Ptr & desc) {
  assert(conn != nullptr);
  assert(desc != nullptr);
  return new CtrlProxy(conn, desc);
}

CtrlProxy::~CtrlProxy() {
  // NOTE(ghilbut): below code makes 
  // "libc++abi.dylib: Pure virtual function called!" runtime error
  // because calling Close() function of Connection interface
  // Close();
}

size_t CtrlProxy::SendText(const std::string & text) const {
  assert(conn_ != nullptr);
  return conn_->SendText(text);
}

size_t CtrlProxy::SendBinary(const std::vector<uint8_t> & bytes) const {
  assert(conn_ != nullptr);
  return conn_->SendBinary(bytes);
}

void CtrlProxy::Close() {
  assert(conn_ != nullptr);
  conn_->Close();
}

void CtrlProxy::BindDelegate(Delegate * delegate) {
  assert(delegate != nullptr);
  delegate_ = delegate;
}

void CtrlProxy::UnbindDelegate() {
  delegate_ = &kNullDelegate;
}

const CtrlDesc & CtrlProxy::desc() const {
  return *desc_;
}

void CtrlProxy::OnText(Connection * conn, const Text & text) {
  assert(conn != nullptr);
  assert(conn == conn_);
  delegate_->OnText(this, text);
}

void CtrlProxy::OnBinary(Connection * conn, const Bytes & bytes) {
  assert(conn != nullptr);
  assert(conn == conn_);
  delegate_->OnBinary(this, bytes);
}

void CtrlProxy::OnClosed(Connection * conn) {
  assert(conn != nullptr);
  assert(conn == conn_);
  delegate_->OnClosed(this);
  conn_->UnbindDelegate();
}

CtrlProxy::CtrlProxy(Connection::Ptr & conn,
                     const CtrlDesc::Ptr & desc)
  : Object()
  , delegate_(&kNullDelegate)
  , conn_(conn)
  , desc_(desc) {

  assert(conn_ != nullptr);
  assert(desc_ != nullptr);
  conn_->BindDelegate(this);

  Json::Value root(Json::objectValue);
  root["user"] = desc_->user;
  root["endpoint"] = desc_->endpoint;
  Json::FastWriter writer;
  const std::string ack = writer.write(root);
  conn_->SendText(ack);
}


}  // namespace ygg
