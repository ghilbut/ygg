#ifndef SERVER_TEST_HTTP_CLIENT_H_
#define SERVER_TEST_HTTP_CLIENT_H_

#include <unistd.h>
#include <mongoose/mongoose.h>
#include <atomic>
#include <functional>
#include <string>
#include <boost/thread.hpp>
#include <vector>


namespace ygg {
namespace server {


class TestHttpClient {
 public:
  TestHttpClient();
  ~TestHttpClient();

  struct mg_connection * Connect(const std::string & address,
                                 const std::string & uri);
  void SendText(struct mg_connection * conn, const std::string & text);
  void SendBinary(struct mg_connection * conn,
                  const std::vector<uint8_t> & bytes);
  void Close(struct mg_connection * conn);


  std::function<void (struct mg_connection*, const std::string&)> on_text_;
  std::function<void (struct mg_connection*, const std::vector<uint8_t>&)> on_binary_;

 private:
  static void callback(struct mg_connection * conn, int event, void * data);
  void DoCallback(struct mg_connection * conn, int event, void * data);


 private:
  struct mg_mgr mgr_;
  std::function<void (struct mg_connection*)> on_connected_;
  std::function<void (struct mg_connection*)> on_closed_;

  std::atomic_bool stop_;
  boost::thread thread_;
};


}  // namespace server
}  // namespace ygg



#endif  // SERVER_TEST_HTTP_CLIENT_H_
