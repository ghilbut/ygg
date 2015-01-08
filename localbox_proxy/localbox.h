#ifndef LOCALBOX_PROXY_LOCALBOX_H_
#define LOCALBOX_PROXY_LOCALBOX_H_

#include "localbox_desc.h"
#include <boost/shared_ptr.hpp>
#include <string>


class HttpWebsocket;

class LocalBox {
	typedef boost::shared_ptr<LocalBox> Ptr;
public:
	
	static Ptr New(HttpWebsocket* ws, const std::string& json);

private:
	LocalBox(const LocalBoxDesc& desc, HttpWebsocket* ws);
	~LocalBox();

	static void Delete(LocalBox* box);

private:
	const LocalBoxDesc desc_;
	HttpWebsocket* ws_;
	
};

#endif  // LOCALBOX_PROXY_LOCALBOX_H_