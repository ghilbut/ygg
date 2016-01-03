#include <gmock/gmock.h>
#include "http_server_websocket_connection.h"
#include <mongoose/mongoose.h>
#include <boost/thread.hpp>
#include <atomic>


namespace ygg {
namespace server {


class HttpServerWebSocketTest : public ::testing::Test {
 public:
  virtual void SetUp() {
    stop_ = false;

    mg_mgr_init(&mgr_, this);
    conn_ = mg_bind(
                &mgr_,
                "8000",
                [](struct mg_connection * conn, int event, void * data) {
                    auto d = conn->mgr->user_data;
                    auto p = reinterpret_cast<HttpServerWebSocketTest*>(d);
                    p->DoHandle(conn, event, data);
                    p->handle_(conn, event, data);
                  }
              );
    mg_set_protocol_http_websocket(conn_);

    boost::thread t([this]() {
                        mutex_.lock();
                        run_cond_.notify_one();
                        mutex_.unlock();

                        while (!stop_) {
                          mg_mgr_poll(&mgr_, 1000);
                        }

                        mg_mgr_free(&mgr_);
                      });
    thread_.swap(t);

    boost::mutex::scoped_lock lock(mutex_);
    run_cond_.wait(lock);
  }

  virtual void TearDown() {
    stop_ = true;
    thread_.join();
  }

 protected:
  void DoHandle(struct mg_connection * conn, int event, void * data) {
    switch (event) {
      case MG_EV_WEBSOCKET_HANDSHAKE_DONE:
        break;
      case MG_EV_WEBSOCKET_FRAME:
        break;
      case MG_EV_CLOSE:
        break;
      default:
        break;
    }
  }


 protected:
  boost::function<void (struct mg_connection*, int, void*)> handle_;

 private:
  std::atomic_bool stop_;

  struct mg_mgr mgr_;
  struct mg_connection * conn_;

  boost::mutex mutex_;
  boost::condition_variable run_cond_;
  boost::condition_variable stop_cond_;
  boost::thread thread_;
};


class MockWebSocketDelegate : public WebSocket::Delegate {
 public:
  MOCK_METHOD2(OnText, void(WebSocket*, const Text&));
  MOCK_METHOD2(OnBinary, void(WebSocket*, const Bytes&));
  MOCK_METHOD1(OnClosed, void(WebSocket*));
};


TEST_F(HttpServerWebSocketTest, test) {

  Connection::Ptr conn;

  //MockWebSocketDelegate mock;

  boost::mutex mutex;
  boost::condition_variable cond;

  handle_ = [&conn, &mutex, &cond](struct mg_connection * conn,
                                 int event,
                                 void * data) {

      if (event == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
        printf("A: %d\n", event);
        boost::mutex::scoped_lock lock(mutex);
        cond.notify_one();
      }
    };


  {
    boost::mutex::scoped_lock lock(mutex);
    boost::chrono::seconds d(10);
    ASSERT_NE(boost::cv_status::timeout, cond.wait_for(lock, d));
  }
  printf("B\n");
}


}  // namespace server
}  // namespace ygg
