#ifndef CORE_CTRL_PROXY_H_
#define CORE_CTRL_PROXY_H_

#include "connection.h"
#include "ctrl_desc.h"
#include <string>


namespace ygg {


class CtrlProxy
  : public Object
  , public Connection::Delegate {

 public:
  typedef boost::intrusive_ptr<CtrlProxy> Ptr;
  typedef BaseDelegate<CtrlProxy> Delegate;

 public:
  static Ptr New(Connection::Ptr & conn, const CtrlDesc::Ptr & desc);

  virtual ~CtrlProxy();

  size_t SendText(const std::string & text) const;
  size_t SendBinary(const std::vector<uint8_t> & bytes) const;
  void Close();

  void BindDelegate(Delegate * delegate);
  void UnbindDelegate();

  const CtrlDesc & desc() const;

  // core::Connection::Delegate
  virtual void OnText(Connection * conn, const Text & text);
  virtual void OnBinary(Connection * conn, const Bytes & bytes);
  virtual void OnClosed(Connection * conn);

 private:
  CtrlProxy(Connection::Ptr & conn, const CtrlDesc::Ptr & desc);


 private:
  Delegate * delegate_;
  Connection::Ptr conn_;
  const CtrlDesc::Ptr desc_;
};


}  // namespace ygg


#endif  // CORE_CTRL_PROXY_H_
