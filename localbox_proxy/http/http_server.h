#ifndef LOCALBOX_PROXY_HTTP_SERVER_H_
#define LOCALBOX_PROXY_HTTP_SERVER_H_

#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <map>


struct mg_server;
struct mg_connection;
enum mg_event;

class HttpServerDelegate;
class HttpWebsocket;


class HttpServer {
public:
    explicit HttpServer(HttpServerDelegate& delegate);
    ~HttpServer();

    void Start(int port);
    void Stop();


private:
    void run();
    static int ev_handler(struct mg_connection *conn, enum mg_event ev);


private:
    struct mg_server *server_;
    HttpServerDelegate& delegate_;
    boost::atomic_bool running_;
    boost::atomic_bool continue_;
    boost::thread thread_;

    typedef std::map<struct mg_connection*, HttpWebsocket*> WSTable;
    WSTable ws_table_;
};

#endif  // LOCALBOX_PROXY_HTTP_SERVER_H_