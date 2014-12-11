#ifndef YGG_DUMMY_BOX_BOX_IMPL_H_
#define YGG_DUMMY_BOX_BOX_IMPL_H_

#include "box_desc.h"
#include "codebase/object.h"
#include "codebase/net/tcp_client.h"


class BoxImpl : public codebase::Object, public codebase::net::TcpClient {
public:
    BoxImpl(IOService& io_service, BoxDesc& desc);
    ~BoxImpl();

protected:
    virtual void OnConnect(const boost::system::error_code& error);
    virtual void OnError(const boost::system::error_code& error);
    virtual void OnRecv(const std::string& data);
    virtual void OnClosed();

private:
    const BoxDesc desc_;
};

#endif  // YGG_DUMMY_BOX_BOX_IMPL_H_