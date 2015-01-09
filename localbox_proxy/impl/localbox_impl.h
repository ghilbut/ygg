#ifndef LOCALBOX_PROXY_IMPL_LOCALBOX_IMPL_H_
#define LOCALBOX_PROXY_IMPL_LOCALBOX_IMPL_H_

#include "http/http_websocket.h"
#include "localbox_desc.h"
#include <boost/noncopyable.hpp>
#include <string>


class LocalBox::Impl : public boost::noncopyable {
private:
    Impl(const LocalBoxDesc& desc, HttpWebsocket &ws);
    ~Impl() {}

public:
    typedef boost::shared_ptr<Impl> Ptr;
    static Ptr New(const std::string& json, HttpWebsocket &ws);
    static void Delete(Impl* impl);


private:
	const LocalBoxDesc desc_;
	HttpWebsocket ws_;
};

#endif  // LOCALBOX_PROXY_IMPL_LOCALBOX_IMPL_H_