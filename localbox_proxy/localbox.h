#ifndef LOCALBOX_PROXY_LOCALBOX_H_
#define LOCALBOX_PROXY_LOCALBOX_H_

#include "localbox_desc.h"
#include <string>


class HttpWebsocket;

class LocalBox {
public:
	static LocalBox* New(HttpWebsocket* ws, const std::string& json);
	static void Delete(LocalBox* box);

private:
	LocalBox(const LocalBoxDesc& desc, HttpWebsocket* ws);
	~LocalBox();

private:
	const LocalBoxDesc desc_;
	HttpWebsocket* ws_;
	
};

#endif  // LOCALBOX_PROXY_LOCALBOX_H_