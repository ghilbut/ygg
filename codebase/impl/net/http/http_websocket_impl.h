#ifndef CODEBASE_IMPL_NET_HTTP_HTTP_WEBSOCKET_IMPL_H_
#define CODEBASE_IMPL_NET_HTTP_HTTP_WEBSOCKET_IMPL_H_

#include "object_impl.h"
#include <string>


struct mg_connection;

namespace codebase {


class HttpWebsocket::Impl : public Object::Impl {
public:
	explicit Impl(struct mg_connection* conn);
    ~Impl();

    void WriteText(const std::string& text) const;
    //void WriteBinary(const char* begin, const size_t size);
    void Close();

private:
	struct mg_connection* conn_;
};


}  // namespace codebase

#endif  // CODEBASE_IMPL_NET_HTTP_HTTP_WEBSOCKET_IMPL_H_