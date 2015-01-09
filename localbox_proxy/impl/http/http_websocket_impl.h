#ifndef LOCALBOX_PROXY_IMPL_HTTP_WEBSOCKET_IMPL_H_
#define LOCALBOX_PROXY_IMPL_HTTP_WEBSOCKET_IMPL_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>


struct mg_connection;


class HttpWebsocket::Impl : public boost::noncopyable {
public:
    explicit Impl(struct mg_connection* conn);
    ~Impl();

    void WriteText(const std::string& text) const;
    //void WriteBinary(const char* begin, const size_t size);
    void Close();

private:
    struct mg_connection* conn_;
};

#endif  // LOCALBOX_PROXY_IMPL_HTTP_WEBSOCKET_IMPL_H_