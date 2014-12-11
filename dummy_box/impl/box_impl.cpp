#include "box.h"
#include "box_impl.h"
#include <boost/bind.hpp>
#include <cassert>


Box::Box(IOService& io_service, BoxDesc& desc)
    : ObjectRef(new BoxImpl(io_service, desc)) {
    // nothing
}

Box::~Box() {
    // nothing
}

void Box::Write(const std::string& text) {
    BoxImpl* impl = Get<BoxImpl>();
    if (impl != NULL) {
        impl->Send(text);
    }
}

void Box::Close() {
    BoxImpl* impl = Get<BoxImpl>();
    if (impl != NULL) {
        impl->Close();
    }
}



BoxImpl::BoxImpl(IOService& io_service, BoxDesc& desc)
    : Object()
    , TcpClient(io_service)
    , desc_(desc) {

    if (desc_.IsEmpty()) {
        printf("[ERROR][BoxImpl::BoxImpl] Description is empty.\n");
        return;        
    }

    Connect(desc.host(), desc.port());
}

BoxImpl::~BoxImpl() {
    // nothing
}

void BoxImpl::OnConnect(const boost::system::error_code& error) {
    if (error) {
        printf("[BoxImpl::OnConnect] Connection Failed. - %s\n", error.message().c_str());
        return;
    }

    printf("[BoxImpl::OnConnect] Connection is opened.\n");
    Send(desc_.Stringify());
}

void BoxImpl::OnError(const boost::system::error_code& error) {
    assert(error);
    printf("[BoxImpl::OnError] %s\n", error.message().c_str());
}

void BoxImpl::OnRecv(const std::string& data) {
    printf("[BoxImpl::OnRead] %s\n", data.c_str());
}

void BoxImpl::OnClosed() {
    printf("[BoxImpl::OnConnect] Connection is closed.\n");
}