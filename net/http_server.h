#ifndef YGG_NET_HTTP_SERVER_H_
#define YGG_NET_HTTP_SERVER_H_

#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <condition_variable>
#include <map>
#include <mutex>


struct mg_server;
struct mg_connection;
enum mg_event;


namespace net {
namespace http {


class ServerDelegate;
class Websocket;

class HttpServer {
public:
	explicit HttpServer(ServerDelegate *delegate = nullptr);
    ~HttpServer();

    void Start(int port);
    void Stop();

	bool IsRunning() const;
	bool IsStopping() const;
	bool IsStopped() const;


private:
    void run();

    static int ev_handler(mg_connection * conn, enum mg_event ev);
    int handle_auth(mg_connection * conn);
    int handle_request(mg_connection * conn);
    int handle_ws_request(mg_connection * conn);
    int handle_close(mg_connection * conn);
    int handle_ws_connect(mg_connection * conn);
    


private:
	class NullDelegate;
	static NullDelegate * const kNullDelegate_;
	static NullDelegate *NewNullDelegate();
	static void DelNullDelegate();

    struct mg_server *server_;
    ServerDelegate *delegate_;

	std::mutex mutex_;
	std::condition_variable cv_;
	boost::atomic_bool is_running_;
	boost::atomic_bool is_stopping_;
	boost::atomic_bool is_stopped_;
    boost::thread thread_;

    typedef std::map<mg_connection*, Websocket> WSTable;
    WSTable ws_table_;
};


}  // namespace http
}  // namespace net

#endif  // YGG_NET_HTTP_SERVER_H_
