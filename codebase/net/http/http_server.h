#ifndef LOCALBOX_PROXY_HTTP_HTTP_SERVER_H_
#define LOCALBOX_PROXY_HTTP_HTTP_SERVER_H_

#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <condition_variable>
#include <map>
#include <mutex>


struct mg_server;
struct mg_connection;
enum mg_event;


namespace codebase {


class HttpServerDelegate;
class HttpWebsocket;

class HttpServer {
public:
	explicit HttpServer(HttpServerDelegate *delegate = nullptr);
    ~HttpServer();

    void Start(int port);
    void Stop();

	bool IsRunning() const;
	bool IsStopping() const;
	bool IsStopped() const;


private:
    void run();

    static int ev_handler(struct mg_connection *conn, enum mg_event ev);
    int handle_auth(struct mg_connection *conn);
    int handle_request(struct mg_connection *conn);
    int handle_ws_request(struct mg_connection *conn);
    int handle_close(struct mg_connection *conn);
    int handle_ws_connect(struct mg_connection *conn);
    


private:
	class NullDelegate;
	static NullDelegate * const kNullDelegate_;
	static NullDelegate *NewNullDelegate();
	static void DelNullDelegate();

    struct mg_server *server_;
    HttpServerDelegate *delegate_;

	std::mutex mutex_;
	std::condition_variable cv_;
	boost::atomic_bool is_running_;
	boost::atomic_bool is_stopping_;
	boost::atomic_bool is_stopped_;
    boost::thread thread_;

    typedef std::map<struct mg_connection*, HttpWebsocket> WSTable;
    WSTable ws_table_;
};


}  // namespace codebase

#endif  // LOCALBOX_PROXY_HTTP_HTTP_SERVER_H_