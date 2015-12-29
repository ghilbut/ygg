#include <gmock/gmock.h>
#include "http_server.h"


using ::testing::_;
using ::testing::InvokeWithoutArgs;


namespace ygg {
namespace server {


static int s_exit_flag = 0;


class HttpServerTest : public ::testing::Test {
 public:
  void SetUp() {
    mg_mgr_init(&mgr_, this);

    stop_ = false;
    boost::thread t(boost::bind(&HttpServerTest::polling, this));
    thread_.swap(t);
  }

  void TearDown() {
    stop_ = true;
    thread_.join();

    mg_mgr_free(&mgr_);
  }


  static void callback(struct mg_connection * nc, int ev, void *ev_data) {
    auto pThis = reinterpret_cast<HttpServerTest*>(nc->mgr->user_data);
    pThis->DoCallback(nc, ev, ev_data);
  }

  void DoCallback(struct mg_connection * nc, int ev, void *ev_data) {
    struct websocket_message *wm = (struct websocket_message *) ev_data;

    if (ev == MG_EV_HTTP_REPLY) {
      return;
    }

    if (ev == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
      mutex_.lock();
      handshake_cond_.notify_one();
      mutex_.unlock();
      return;
    }

    if (ev == MG_EV_WEBSOCKET_FRAME) {
      return;
    }
  }


  void polling() {
    while (!stop_) {
      mg_mgr_poll(&mgr_, 1000);
    }
  }


 protected:
  struct mg_mgr mgr_;

  std::atomic_bool stop_;

  boost::mutex mutex_;
  boost::condition_variable request_cond_;
  boost::condition_variable handshake_cond_;
  boost::condition_variable message_cond_;
  boost::condition_variable close_cond_;
  boost::thread thread_;
};


class MockHttpServerDelegate : public HttpServer::Delegate {
 public:
  MOCK_METHOD2(OnRequest, void(struct mg_connection*, struct http_message*));
  MOCK_METHOD1(OnWSOpened, void(struct mg_connection*));
  MOCK_METHOD2(OnWSMessage, void(struct mg_connection*,
                                 struct websocket_message*));
  MOCK_METHOD1(OnWSClosed, void(struct mg_connection*));
};





static int s_show_headers = 0;

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_CONNECT:
      if (* (int *) ev_data != 0) {
        fprintf(stderr, "connect() failed: %s\n", strerror(* (int *) ev_data));
        s_exit_flag = 1;
      }
      break;
    case MG_EV_HTTP_REPLY:
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      if (s_show_headers) {
        fwrite(hm->message.p, 1, hm->message.len, stdout);
      } else {
        fwrite(hm->body.p, 1, hm->body.len, stdout);
      }
      putchar('\n');
      s_exit_flag = 1;
      break;
    default:
      break;
  }
}


void DoStop() {
  s_exit_flag = 1;
}





TEST_F(HttpServerTest, test1) {

  MockHttpServerDelegate mock;

  ON_CALL(mock, OnRequest(_, _))
    .WillByDefault(InvokeWithoutArgs([this]() {
        boost::mutex::scoped_lock lock(mutex_);
        request_cond_.notify_one();
      }));

  EXPECT_CALL(mock, OnRequest(_, _));

  HttpServer s;
  s.BindDelegate(&mock);
  s.Start();



  mg_connect_http(&mgr_, ev_handler, "http://127.0.0.1:8000/B/methods", NULL, NULL);
  {
    boost::mutex::scoped_lock lock(mutex_);
    request_cond_.wait(lock);
  }



  s.Stop();
}







TEST_F(HttpServerTest, test2) {

  MockHttpServerDelegate mock;
  EXPECT_CALL(mock, OnWSOpened(_));
  auto f_message = InvokeWithoutArgs([this]() {
      boost::mutex::scoped_lock lock(mutex_);
      message_cond_.notify_all();
    });
  EXPECT_CALL(mock, OnWSMessage(_, _)).WillOnce(f_message);
  auto f_close = InvokeWithoutArgs([this]() {
      boost::mutex::scoped_lock lock(mutex_);
      close_cond_.notify_all();
    });
  EXPECT_CALL(mock, OnWSClosed(_)).WillOnce(f_close);

  HttpServer s;
  s.BindDelegate(&mock);
  s.Start();



  const char *local_addr = "127.0.0.1:8000";
  char buf[20] = "";

  struct mg_connection * nc = mg_connect(&mgr_, local_addr, callback);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = buf;
  mg_send_websocket_handshake(nc, "/ws", NULL);
  {
    boost::mutex::scoped_lock lock(mutex_);
    handshake_cond_.wait(lock);
  }
 
  struct mg_str h[2];
  h[0].p = "h";
  h[0].len = 1;
  h[1].p = "i";
  h[1].len = 1;
  mg_send_websocket_framev(nc, WEBSOCKET_OP_TEXT, h, 2);
  {
    boost::mutex::scoped_lock lock(mutex_);
    message_cond_.wait(lock);
  }

  mg_send_websocket_frame(nc, WEBSOCKET_OP_CLOSE, NULL, 0);
  {
    boost::mutex::scoped_lock lock(mutex_);
    close_cond_.wait(lock);
  }



  s.Stop();
}






}  // namespace server
}  // namespace ygg
