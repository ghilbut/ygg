#include <gmock/gmock.h>
#include "http_server.h"
#include "http_server_websocket_connection.h"
#include "test/vars.h"
#include <mongoose/mongoose.h>
#include <boost/thread.hpp>
#include <atomic>
#include <functional>


using ::testing::_;
using ::testing::ContainerEq;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::StrEq;


namespace ygg {
namespace server {


static const char * const kAddress = "127.0.0.1:8000";
static const char * const kUri = "/ws";


class HttpServerWebSocketTest
  : public ::testing::Test
  , public HttpServer::Delegate {

 public:
  HttpServerWebSocketTest() {
    server_.BindDelegate(this);
  }

  ~HttpServerWebSocketTest() {
    server_.UnbindDelegate();
  }

 public:
  // HttpServer::Delegate
  virtual void OnRequest(struct mg_connection*, struct http_message*) {
    // nothing
  }
  virtual void OnWebSocket(Connection::Ptr ws, const std::string & uri) {
    on_server_websocket_(ws, uri);
  }

  // ::testing::Test
 protected:
  virtual void SetUp() {
    on_server_websocket_ = [](Connection::Ptr, const std::string&) {};
    server_.Start();

    stop_ = false;
    mg_mgr_init(&mgr_, this);

    on_client_handshake_ = [](struct mg_connection*) {};
    on_client_text_ = [](struct mg_connection*, const Text&) {};
    on_client_binary_ = [](struct mg_connection*, const Bytes&) {};
    on_client_closed_ = [](struct mg_connection*) {};

    boost::thread t([this]() {
        while (!stop_) {
          mg_mgr_poll(&mgr_, 1000);
        }
      });
    thread_.swap(t);
  }

  virtual void TearDown() {
    stop_ = true;
    thread_.join();
    mg_mgr_free(&mgr_);

    server_.Stop();
  }


 protected:
  struct mg_connection * OpenClient() {

    boost::mutex mutex;
    boost::condition_variable cond;

    struct mg_connection * client;
    on_client_handshake_ = [&](struct mg_connection * ws) {
        ASSERT_EQ(client, ws);
        boost::mutex::scoped_lock lock(mutex);
        cond.notify_one();
      };

    client = mg_connect(&mgr_, kAddress, callback);
    mg_set_protocol_http_websocket(client);
    mg_send_websocket_handshake(client, kUri, NULL);
    {
      boost::chrono::seconds d(10);
      boost::mutex::scoped_lock lock(mutex);
      auto timeout = cond.wait_for(lock, d);
      if (boost::cv_status::timeout == timeout) {
        mg_send_websocket_frame(client, WEBSOCKET_OP_CLOSE, nullptr, 0);
        return nullptr;
      }
    }

    return client;
  }

  void CloseClient(struct mg_connection * const client) {

    boost::mutex mutex;
    boost::condition_variable cond;

    on_client_closed_ = [&](struct mg_connection * ws) {
        ASSERT_EQ(client, ws);
        boost::mutex::scoped_lock lock(mutex);
        cond.notify_one();
      };

    mg_send_websocket_frame(client, WEBSOCKET_OP_CLOSE, nullptr, 0);
    {
      const boost::chrono::seconds d(10);
      boost::mutex::scoped_lock lock(mutex);
      const auto timeout = cond.wait_for(lock, d);
      ASSERT_EQ(boost::cv_status::no_timeout, timeout);
    }
  }


 private:
  static void callback(struct mg_connection * client,
                       int event,
                       void * data) {
    auto d = client->mgr->user_data;
    auto p = reinterpret_cast<HttpServerWebSocketTest*>(d);
    p->DoCallback(client, event, data);
  }

  void DoCallback(struct mg_connection * client,
                  int event,
                  void * data) {

    if (event == MG_EV_CONNECT) {
      if (* (int *) data != 0) {
        fprintf(stderr, "connect() failed: %s\n", strerror(* (int*) data));
      }
      return;
    }

    if (event == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
      on_client_handshake_(client);
      return;
    }

    if (event == MG_EV_WEBSOCKET_FRAME) {
      const auto wm = reinterpret_cast<struct websocket_message*>(data);

      if ((wm->flags & WEBSOCKET_OP_TEXT) == WEBSOCKET_OP_TEXT) {
        const Text text(wm->data, wm->data + wm->size);
        on_client_text_(client, text);
        return;
      }

      if ((wm->flags & WEBSOCKET_OP_BINARY) == WEBSOCKET_OP_BINARY) {
        const Bytes bytes(wm->data, wm->data + wm->size);
        on_client_binary_(client, bytes);
        return;
      }

      // nothing
      return;
    }

    if (event == MG_EV_CLOSE) {
      if ((client->flags & MG_F_IS_WEBSOCKET) == MG_F_IS_WEBSOCKET) {
        on_client_closed_(client);
      }
      return;
    }
  }


 protected:
  HttpServer server_;
  std::function<void (Connection::Ptr, const std::string&)> on_server_websocket_;

  struct mg_mgr mgr_;
  std::atomic_bool stop_;
  std::function<void (struct mg_connection*)> on_client_handshake_;
  std::function<void (struct mg_connection*, const Text&)> on_client_text_;
  std::function<void (struct mg_connection*, const Bytes&)> on_client_binary_;
  std::function<void (struct mg_connection*)> on_client_closed_;

  boost::thread thread_;
};


class MockWebSocketDelegate : public WebSocket::Delegate {
 public:
  MOCK_METHOD2(OnText, void(Connection*, const Text&));
  MOCK_METHOD2(OnBinary, void(Connection*, const Bytes&));
  MOCK_METHOD1(OnClosed, void(Connection*));
};


TEST_F(HttpServerWebSocketTest, websocket_connect_and_close) {

  Connection::Ptr target;

  // callback when websocket connected on server side.
  on_server_websocket_ = [&](Connection::Ptr ws, const std::string & uri) {
      ASSERT_NE(nullptr, ws);
      ASSERT_EQ(kUri, uri);
      target = ws;
    };

  const auto client = OpenClient();
  ASSERT_NE(nullptr, client);

  boost::mutex mutex;
  boost::condition_variable cond;

  // mock for websocket connection delegate.
  MockWebSocketDelegate mock_ws;
  EXPECT_CALL(mock_ws, OnClosed(_))
    .WillOnce(Invoke([&](Connection * ws) {
        ASSERT_EQ(target, ws);
        boost::mutex::scoped_lock lock(mutex);
        cond.notify_one();
      }));

  // bind mock to websocket connection.
  target->BindDelegate(&mock_ws);

  // close client connection.
  mg_send_websocket_frame(client, WEBSOCKET_OP_CLOSE, nullptr, 0);

  // check server side websocket closed 
  {
    const boost::chrono::seconds d(10);
    boost::mutex::scoped_lock lock(mutex);
    const auto timeout = cond.wait_for(lock, d);
    ASSERT_EQ(boost::cv_status::no_timeout, timeout);
  }

  // unbind mock from websocket connection.
  target->UnbindDelegate();
}


TEST_F(HttpServerWebSocketTest, recv_text) {

  Connection::Ptr target;

  on_server_websocket_ = [&](Connection::Ptr ws, const std::string & uri) {
      target = ws;
    };

  const auto client = OpenClient();
  ASSERT_NE(nullptr, client);

  boost::mutex mutex;
  boost::condition_variable cond;

  const auto expected(test::GetRandomString());

  MockWebSocketDelegate mock_ws;
  EXPECT_CALL(mock_ws, OnText(_, StrEq(expected)))
    .WillOnce(InvokeWithoutArgs([&]() {
        boost::mutex::scoped_lock lock(mutex);
        cond.notify_one();
      }));

  target->BindDelegate(&mock_ws);

  mg_send_websocket_frame(client,
                          WEBSOCKET_OP_TEXT,
                          &expected[0],
                          expected.length());
  {
    boost::mutex::scoped_lock lock(mutex);
    cond.wait_for(lock, boost::chrono::seconds{10});
  }

  target->UnbindDelegate();

  CloseClient(client);
}


TEST_F(HttpServerWebSocketTest, send_text) {

  Connection::Ptr target;

  on_server_websocket_ = [&](Connection::Ptr ws, const std::string & uri) {
      target = ws;
    };

  const auto client = OpenClient();
  ASSERT_NE(nullptr, client);

  boost::mutex mutex;
  boost::condition_variable cond;

  const auto expected(test::GetRandomString());

  on_client_text_ = [&](struct mg_connection * ws, const Text & text) {
      ASSERT_EQ(client, ws);
      ASSERT_EQ(expected, text);
      boost::mutex::scoped_lock lock(mutex);
      cond.notify_one();
    };

  target->SendText(expected);
  {
    const boost::chrono::seconds d(10);
    boost::mutex::scoped_lock lock(mutex);
    const auto timeout = cond.wait_for(lock, d);
    ASSERT_EQ(boost::cv_status::no_timeout, timeout);
  }

  target->UnbindDelegate();

  CloseClient(client);
}


TEST_F(HttpServerWebSocketTest, recv_binary) {

  Connection::Ptr target;

  on_server_websocket_ = [&](Connection::Ptr ws, const std::string & uri) {
      target = ws;
    };

  const auto client = OpenClient();
  ASSERT_NE(nullptr, client);

  boost::mutex mutex;
  boost::condition_variable cond;

  const auto expected(test::GetRandomBytes());

  MockWebSocketDelegate mock_ws;
  EXPECT_CALL(mock_ws, OnBinary(_, ContainerEq(expected)))
    .WillOnce(InvokeWithoutArgs([&]() {
        boost::mutex::scoped_lock lock(mutex);
        cond.notify_one();
      }));

  target->BindDelegate(&mock_ws);

  mg_send_websocket_frame(client,
                          WEBSOCKET_OP_BINARY,
                          &expected[0],
                          expected.size());
  {
    boost::mutex::scoped_lock lock(mutex);
    cond.wait_for(lock, boost::chrono::seconds{10});
  }

  target->UnbindDelegate();

  CloseClient(client);
}


TEST_F(HttpServerWebSocketTest, send_binary) {

  Connection::Ptr target;

  on_server_websocket_ = [&](Connection::Ptr ws, const std::string & uri) {
      target = ws;
    };

  const auto client = OpenClient();
  ASSERT_NE(nullptr, client);

  boost::mutex mutex;
  boost::condition_variable cond;

  const auto expected(test::GetRandomBytes());

  on_client_binary_ = [&](struct mg_connection * ws, const Bytes& bytes) {
      ASSERT_EQ(client, ws);
      ASSERT_EQ(expected, bytes);
      boost::mutex::scoped_lock lock(mutex);
      cond.notify_one();
    };

  target->SendBinary(expected);
  {
    const boost::chrono::seconds d(10);
    boost::mutex::scoped_lock lock(mutex);
    const auto timeout = cond.wait_for(lock, d);
    ASSERT_EQ(boost::cv_status::no_timeout, timeout);
  }

  target->UnbindDelegate();

  CloseClient(client);
}


}  // namespace server
}  // namespace ygg
