#ifndef LOCALBOX_PROXY_HTTP_WEBSOCKET_H_
#define LOCALBOX_PROXY_HTTP_WEBSOCKET_H_

#include <boost/shared_ptr.hpp>
#include <string>


class HttpWebsocket {
public:
    HttpWebsocket() {}
    ~HttpWebsocket() {}

    HttpWebsocket(const HttpWebsocket& other);
    explicit HttpWebsocket(struct mg_connection* conn);

    HttpWebsocket& operator= (const HttpWebsocket& other);
    bool operator== (const HttpWebsocket& other) const;
    bool operator!= (const HttpWebsocket& other) const;

    bool operator< (const HttpWebsocket& other) const;

    bool IsNull() const;

    void WriteText(const std::string& text) const;
    //void WriteBinary(const char* begin, const size_t size);
    void Close();

private:
    class Impl;
    boost::shared_ptr<Impl> impl_;
};

#endif  // LOCALBOX_PROXY_HTTP_WEBSOCKET_H_