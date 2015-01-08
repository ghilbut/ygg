#ifndef LOCALBOX_PROXY_HTTP_WEBSOCKET_H_
#define LOCALBOX_PROXY_HTTP_WEBSOCKET_H_

#include <string>


struct mg_connection;


class HttpWebsocket {
public:
    explicit HttpWebsocket(struct mg_connection* conn);
    ~HttpWebsocket();

    void WriteText(const std::string& text) const;
    //void WriteBinary(const char* begin, const size_t size);

private:
    struct mg_connection* conn_;
};

#endif  // LOCALBOX_PROXY_HTTP_WEBSOCKET_H_