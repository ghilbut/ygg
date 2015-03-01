#ifndef LOCALBOX_PROXY_IMPL_LOCALBOX_IMPL_H_
#define LOCALBOX_PROXY_IMPL_LOCALBOX_IMPL_H_

#include "http.h"
#include "localbox_desc.h"
#include "codebase/net/http/http_websocket.h"
#include "codebase/object_impl.h"
#include <string>


class LocalBox::Impl : public Object::Impl {
	Impl(const LocalBoxDesc &desc, codebase::HttpWebsocket &ws);
	~Impl() {}

public:
	static Impl* New(const std::string &json, codebase::HttpWebsocket &ws);
	// static void Delete(Impl* impl);


private:
	const LocalBoxDesc desc_;
	codebase::HttpWebsocket ws_;
};

#endif  // LOCALBOX_PROXY_IMPL_LOCALBOX_IMPL_H_