#ifndef YGG_SERVER_HTTP_SERVER_H_
#define YGG_SERVER_HTTP_SERVER_H_

// NOT(ghilbut): boost thread make undeclared 'getpagesize' function error.
//               include unistd.h solve this problem.
#include <unistd.h> 
#include <mongoose/mongoose.h>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <atomic>


namespace ygg {
namespace server {


class HttpServer {
 public:
  enum Status {
    kStopped = 0,
    kStarting,
    kRunning,
    kStopping,
  };

  HttpServer();
  ~HttpServer();

  bool Start();
  void Stop();


 private:
  void polling();
  void DoHandle(struct mg_connection * conn, int event);
  static void ev_handler(struct mg_connection * conn,
                         int event,
                         void * data);


 private:
  std::atomic_bool stop_;
  //volatile bool stop_;
  std::atomic<Status> status_;

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
