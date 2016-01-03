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
 public:
  void SetUp() {
    mg_mgr_init(&mgr_, this);

    stop_ = false;
    recv_text_.clear();
    recv_bytes_.clear();
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

    if (ev == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
      boost::mutex::scoped_lock lock(mutex_);
      handshake_cond_.notify_one();
      return;
    }

    if (ev == MG_EV_WEBSOCKET_FRAME) {
      struct websocket_message * wm = (struct websocket_message *) ev_data;
      if ((wm->flags & WEBSOCKET_OP_TEXT) == WEBSOCKET_OP_TEXT) {
        Text text(wm->data, wm->data + wm->size);
        recv_text_ = text;
        boost::mutex::scoped_lock lock(mutex_);
        text_cond_.notify_one();
      }
      else if ((wm->flags & WEBSOCKET_OP_BINARY) == WEBSOCKET_OP_BINARY) {
        Bytes bytes(wm->data, wm->data + wm->size);
        recv_bytes_ = bytes;
        boost::mutex::scoped_lock lock(mutex_);
        binary_cond_.notify_one();
      }
      else {
        // nothing
      }
      return;
    }

    if (ev == MG_EV_CLOSE) {
      if ((nc->flags & MG_F_IS_WEBSOCKET) == MG_F_IS_WEBSOCKET) {
        boost::mutex::scoped_lock lock(mutex_);
        close_cond_.notify_one();
      }
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

  Text recv_text_;
  Bytes recv_bytes_;

  boost::mutex mutex_;
  boost::condition_variable request_cond_;
  boost::condition_variable handshake_cond_;
  boost::condition_variable text_cond_;
  boost::condition_variable binary_cond_;
  boost::condition_variable close_cond_;
  boost::thread thread_;
};



class MockHttpServerDelegate : public HttpServer::Delegate {
 public:
  MOCK_METHOD2(OnRequest, void(struct mg_connection*, struct http_message*));
  MOCK_METHOD1(OnWebSocket, void(Connection::Ptr));
};

class MockWebSocketDelegate : public WebSocket::Delegate {
 public:
  MOCK_METHOD2(OnText, void(Connection*, const Text&));
  MOCK_METHOD2(OnBinary, void(Connection*, const Bytes&));
  MOCK_METHOD1(OnClosed, void(Connection*));
};



TEST_F(HttpServerTest, test_request) {

  MockHttpServerDelegate mock;

  ON_CALL(mock, OnRequest(_, _))
    .WillByDefault(InvokeWithoutArgs([this]() {
        boost::mutex::scoped_lock lock(mutex_);
        request_cond_.notify_all();
      }));

  EXPECT_CALL(mock, OnRequest(_, _));

  HttpServer s;
  s.BindDelegate(&mock);
  s.Start();



  static const char * kUrl = "http://127.0.0.1:8000/B/methods";
  mg_connect_http(&mgr_, callback, kUrl, NULL, NULL);
  {
    boost::mutex::scoped_lock lock(mutex_);
    //request_cond_.wait(lock);
    boost::chrono::seconds d(10);
    EXPECT_TRUE(boost::cv_status::timeout != request_cond_.wait_for(lock, d));
  }



  s.Stop();
}





TEST_F(HttpServerTest, test_websocket_connect_and_close) {

  Connection::Ptr target;

  // mock for http server delegate.
  MockHttpServerDelegate mock_server;
  EXPECT_CALL(mock_server, OnWebSocket(_))
    .WillOnce(Invoke([&target](Connection::Ptr ws) {
        ASSERT_NE(nullptr, ws);
        target = ws;
      }));

  // bind mock to http server and start server.
  HttpServer s;
  s.BindDelegate(&mock_server);
  s.Start();

  // make and connect client websocket to server.
  struct mg_connection * client = mg_connect(&mgr_, kAddress, callback);
  mg_set_protocol_http_websocket(client);
  mg_send_websocket_handshake(client, "/ws", NULL);
  {
    boost::chrono::seconds d(10);
    boost::mutex::scoped_lock lock(mutex_);
    auto timeout = handshake_cond_.wait_for(lock, d);
    ASSERT_EQ(boost::cv_status::no_timeout, timeout);
  }

  // mock for websocket connection delegate.
  MockWebSocketDelegate mock_ws;
  EXPECT_CALL(mock_ws, OnClosed(_))
    .WillOnce(Invoke([&target](Connection * ws) {
        ASSERT_EQ(target, ws);
      }));

  // bind mock to wehsocket connection.
  target->BindDelegate(&mock_ws);

  // client closes connection.
  mg_send_websocket_frame(client, WEBSOCKET_OP_CLOSE, nullptr, 0);
  {
    boost::chrono::seconds d(10);
    boost::mutex::scoped_lock lock(mutex_);
    auto timeout = close_cond_.wait_for(lock, d);
    ASSERT_EQ(boost::cv_status::no_timeout, timeout);
  }

  // stop server.
  s.Stop();
}


TEST_F(HttpServerTest, test_recv_text) {

  Connection::Ptr target;

  MockHttpServerDelegate mock_server;
  EXPECT_CALL(mock_server, OnWebSocket(_))
    .WillOnce(Invoke([&target](Connection::Ptr ws) {
        ASSERT_NE(nullptr, ws);
        target = ws;
      }));

  HttpServer s;
  s.BindDelegate(&mock_server);
  s.Start();

  struct mg_connection * client = mg_connect(&mgr_, kAddress, callback);
  mg_set_protocol_http_websocket(client);
  mg_send_websocket_handshake(client, "/ws", NULL);
  {
    boost::mutex::scoped_lock lock(mutex_);
    handshake_cond_.wait(lock);
  }

  const std::string expected(test::GetRandomString());

  boost::mutex mutex;
  boost::condition_variable message_cond;

  MockWebSocketDelegate mock_ws;
  EXPECT_CALL(mock_ws, OnText(_, expected))
    .WillOnce(InvokeWithoutArgs([&mutex, &message_cond]() {
        boost::mutex::scoped_lock lock(mutex);
        message_cond.notify_one();
      }));
  EXPECT_CALL(mock_ws, OnClosed(_));

  target->BindDelegate(&mock_ws);

  mg_send_websocket_frame(client,
                          WEBSOCKET_OP_TEXT,
                          &expected[0],
                          expected.length());
  {
    boost::mutex::scoped_lock lock(mutex);
    message_cond.wait(lock);
  }

  mg_send_websocket_frame(client, WEBSOCKET_OP_CLOSE, nullptr, 0);
  {
    boost::mutex::scoped_lock lock(mutex_);
    close_cond_.wait(lock);
  }


  s.Stop();



/*
  MockHttpServerDelegate mock;
  EXPECT_CALL(mock, OnWebSocket(_));

  HttpServer s;
  s.BindDelegate(&mock);
  s.Start();



  char buf[20] = "";

  struct mg_connection * nc = mg_connect(&mgr_, kAddress, callback);
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

  mg_send_websocket_frame(nc, WEBSOCKET_OP_CLOSE, NULL, 0);


  s.Stop();
*/
}


TEST_F(HttpServerTest, test_send_text) {

  Connection::Ptr target;

  MockHttpServerDelegate mock_server;
  EXPECT_CALL(mock_server, OnWebSocket(_))
    .WillOnce(Invoke([&target](Connection::Ptr ws) {
        ASSERT_NE(nullptr, ws);
        target = ws;
      }));

  HttpServer s;
  s.BindDelegate(&mock_server);
  s.Start();

  struct mg_connection * client = mg_connect(&mgr_, kAddress, callback);
  mg_set_protocol_http_websocket(client);
  mg_send_websocket_handshake(client, "/ws", NULL);
  {
    boost::mutex::scoped_lock lock(mutex_);
    handshake_cond_.wait(lock);
  }

  const std::string expected(test::GetRandomString());
  target->SendText(expected);
  {
    boost::chrono::seconds d(10);
    boost::mutex::scoped_lock lock(mutex_);
    auto timeout = text_cond_.wait_for(lock, d);
    ASSERT_EQ(boost::cv_status::no_timeout, timeout);
    ASSERT_EQ(expected, recv_text_);
  }

  mg_send_websocket_frame(client, WEBSOCKET_OP_CLOSE, nullptr, 0);
  {
    boost::chrono::seconds d(10);
    boost::mutex::scoped_lock lock(mutex_);
    auto timeout = close_cond_.wait_for(lock, d);
    ASSERT_EQ(boost::cv_status::no_timeout, timeout);
  }


  s.Stop();
}


}  // namespace server
}  // namespace ygg
