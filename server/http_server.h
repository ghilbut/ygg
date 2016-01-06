#ifndef YGG_SERVER_HTTP_SERVER_H_
#define YGG_SERVER_HTTP_SERVER_H_

#include "http_server_websocket_connection.h"
// NOT(ghilbut): boost thread make undeclared 'getpagesize' function error.
//               include unistd.h solve this problem.
#include <unistd.h> 
#include <mongoose/mongoose.h>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <atomic>
#include <unordered_map>


namespace ygg {
namespace server {


class HttpServer {
 public:
  class Delegate {
   public:
    virtual void OnRequest(struct mg_connection * conn,
                           struct http_message * msg) = 0;
    virtual void OnWebSocket(Connection::Ptr ws, const std::string & uri) = 0;

   protected:
    virtual ~Delegate() {}
  };

 public:
  enum Status {
    kStopped = 0,
    kStarting,
    kRunning,
    kStopping,
  };

  HttpServer();
  ~HttpServer();

  void BindDelegate(Delegate * delegate);
  void UnbindDelegate();

  bool Start();
  void Stop();


 private:
  void polling();
  void DoHandle(struct mg_connection * conn, int event, void * data);
  static void ev_handler(struct mg_connection * conn,
                         int event,
                         void * data);


 private:
  Delegate * delegate_;

  std::atomic_bool stop_;
  std::atomic<Status> status_;

  std::unordered_map<struct mg_connection*, Connection::Ptr> ws_list_;
  std::unordered_map<struct mg_connection*, std::string> ws_uri_list_;

  struct mg_mgr mgr_;
  struct mg_connection * conn_;

  boost::mutex mutex_;
  boost::condition_variable running_cond_;
  boost::condition_variable stopped_cond_;
  boost::thread thread_;
};


}  // namespace server
}  // namespace ygg


#endif  // YGG_SERVER_HTTP_SERVER_H_
