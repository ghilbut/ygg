#ifndef CODEBASE_NET_HTTP_HTTP_WEBSOCKET_H_
#define CODEBASE_NET_HTTP_HTTP_WEBSOCKET_H_

#include "codebase/object.h"
#include <string>


struct mg_connection;

namespace codebase {


class HttpWebsocket : public Object {
public:
	HttpWebsocket() {}
	~HttpWebsocket() {}

	explicit HttpWebsocket(struct mg_connection* conn);

	void WriteText(const std::string& text) const;
	//void WriteBinary(const char* begin, const size_t size);
	void Close();

private:
	class Impl;
};


}  // namespace codebase

#endif  // CODEBASE_NET_HTTP_HTTP_WEBSOCKET_H_