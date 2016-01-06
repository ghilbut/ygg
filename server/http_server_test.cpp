#include <gmock/gmock.h>
#include "http_server.h"
#include "test/vars.h"


using ::testing::_;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;


namespace ygg {
namespace server {


static const char * const kAddress = "127.0.0.1:8000";


class HttpServerTest : public ::testing::Test {
 protected:
  void SetUp() {
    mg_mgr_init(&mgr_, this);

    stop_ = false;
    boost::thread t([this]() {
        while (!stop_) {
          mg_mgr_poll(&mgr_, 1000);
        }
        mg_mgr_free(&mgr_);
      });
    thread_.swap(t);
  }

  void TearDown() {
    stop_ = true;
    thread_.join();
  }

  static void callback(struct mg_connection * nc, int ev, void *ev_data) {
    auto pThis = reinterpret_cast<HttpServerTest*>(nc->mgr->user_data);
    pThis->DoCallback(nc, ev, ev_data);
  }

 private:
  void DoCallback(struct mg_connection * nc, int ev, void *ev_data) {

    if (ev == MG_EV_CONNECT) {
      if (* (int *) ev_data != 0) {
        fprintf(stderr, "connect() failed: %s\n", strerror(* (int*) ev_data));
      }
      return;
    }

    if (ev == MG_EV_HTTP_REPLY) {
      struct http_message *hm = (struct http_message *) ev_data;
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      //if (s_show_headers) {
      //  fwrite(hm->message.p, 1, hm->message.len, stdout);
      //} else {
        fwrite(hm->body.p, 1, hm->body.len, stdout);
      //}
      putchar('\n');
      return;
    }
  }


 protected:
  struct mg_mgr mgr_;

 private:
  std::atomic_bool stop_;
  boost::thread thread_;
};



class MockHttpServerDelegate : public HttpServer::Delegate {
 public:
  MOCK_METHOD2(OnRequest, void(struct mg_connection*, struct http_message*));
  MOCK_METHOD2(OnWebSocket, void(Connection::Ptr, const std::string&));
};



TEST_F(HttpServerTest, test_request) {

  boost::mutex mutex;
  boost::condition_variable cond;

  MockHttpServerDelegate mock;

  ON_CALL(mock, OnRequest(_, _))
    .WillByDefault(InvokeWithoutArgs([&]() {
        boost::mutex::scoped_lock lock(mutex);
        cond.notify_all();
      }));

  EXPECT_CALL(mock, OnRequest(_, _));

  HttpServer s;
  s.BindDelegate(&mock);
  s.Start();



  static const char * kUrl = "http://127.0.0.1:8000/B/methods";
  mg_connect_http(&mgr_, callback, kUrl, NULL, NULL);
  {
    const boost::chrono::seconds d(10);
    boost::mutex::scoped_lock lock(mutex);
    const auto timeout = cond.wait_for(lock, d);
    ASSERT_EQ(boost::cv_status::no_timeout, timeout);
  }



  s.Stop();
}


}  // namespace server
}  // namespace ygg
