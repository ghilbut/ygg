#include "http_client.h"
#include "test/vars.h"
#include <gmock/gmock.h>
#include <mongoose/mongoose.h>
#include <map>


namespace ygg {
namespace server {


class TestHttpClientTest : public ::testing::Test {
 protected:
  virtual void SetUp() {

    std::atomic_bool is_run(false);

    on_server_text_ = [](const std::string&) {};
    on_server_binary_ = [](const std::vector<uint8_t>&) {};

    mg_mgr_init(&mgr_, this);
    server_ = mg_bind(&mgr_, "8000", callback);
    target_ = nullptr;
    mg_set_protocol_http_websocket(server_);

    boost::mutex mutex;
    boost::condition_variable cond;

    stop_ = false;
    boost::thread t([&]() {
        mutex.lock();
        is_run = true;
        cond.notify_one();
        mutex.unlock();

        while (!stop_) {
          mg_mgr_poll(&mgr_, 1000);
        }

        mg_mgr_free(&mgr_);
      });
    thread_.swap(t);

    while (!is_run) {
      const boost::chrono::seconds d(1);
      boost::mutex::scoped_lock lock(mutex);
      cond.wait_for(lock, d);
    }

    conn_ = client_.Connect("127.0.0.1:8000", "/ws");
  }

  virtual void TearDown() {
    client_.Close(conn_);

    stop_ = true;
    thread_.join();
  }

  void SendText(const std::string & text) {
    assert(target_ != nullptr);
    mg_send_websocket_frame(target_,
                            WEBSOCKET_OP_TEXT,
                            text.c_str(),
                            text.length());
  }

  void SendBinary(const std::vector<uint8_t> & bytes) {
    assert(target_ != nullptr);
    mg_send_websocket_frame(target_,
                            WEBSOCKET_OP_BINARY,
                            &bytes[0],
                            bytes.size());
  }

 private:
  static void callback(struct mg_connection * conn, int event, void * data) {
    auto d = conn->mgr->user_data;
    auto p = reinterpret_cast<TestHttpClientTest*>(d);
    p->DoCallback(conn, event, data);
  }

  void DoCallback(struct mg_connection * conn, int event, void * data) {
    if (event == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
      target_ = conn;
      return;
    }
    if (event == MG_EV_WEBSOCKET_FRAME) {
      auto wm = reinterpret_cast<websocket_message*>(data);
      if ((wm->flags & WEBSOCKET_OP_TEXT) == WEBSOCKET_OP_TEXT) {
        const std::string text(wm->data, wm->data + wm->size);
        on_server_text_(text);
        return;
      }
      if ((wm->flags & WEBSOCKET_OP_BINARY) == WEBSOCKET_OP_BINARY) {
        const std::vector<uint8_t> bytes(wm->data, wm->data + wm->size);
        on_server_binary_(bytes);
        return;
      }
    }
  }


 protected:
  TestHttpClient client_;
  struct mg_connection * conn_;

  std::function<void (const std::string&)> on_server_text_;
  std::function<void (const std::vector<uint8_t>&)> on_server_binary_;

 private:
  mg_mgr mgr_;
  struct mg_connection * server_;
  struct mg_connection * target_;

  std::atomic_bool stop_;
  boost::thread thread_;
};


TEST_F(TestHttpClientTest, connect) {
  ASSERT_NE(nullptr, conn_);
}


TEST_F(TestHttpClientTest, send_text) {

  boost::mutex mutex;
  boost::condition_variable cond;

  const std::string expected(test::GetRandomString());

  auto f = [&](const std::string & text) {
      ASSERT_EQ(expected, text);
      boost::mutex::scoped_lock lock(mutex);
      cond.notify_one();
    };
  on_server_text_ = f; 

  client_.SendText(conn_, expected);

  const boost::chrono::seconds d(10);
  boost::mutex::scoped_lock lock(mutex);
  const auto timeout = cond.wait_for(lock, d);
  ASSERT_EQ(boost::cv_status::no_timeout, timeout);
}

TEST_F(TestHttpClientTest, recv_text) {

  boost::mutex mutex;
  boost::condition_variable cond;

  const std::string expected(test::GetRandomString());

  auto f = [&](struct mg_connection * conn, const std::string & text) {
      ASSERT_EQ(conn_, conn);
      ASSERT_EQ(expected, text);
      boost::mutex::scoped_lock lock(mutex);
      cond.notify_one();
    };
  client_.on_text_ = f;

  SendText(expected);

  const boost::chrono::seconds d(10);
  boost::mutex::scoped_lock lock(mutex);
  const auto timeout = cond.wait_for(lock, d);
  ASSERT_EQ(boost::cv_status::no_timeout, timeout);
}

TEST_F(TestHttpClientTest, send_binary) {

  boost::mutex mutex;
  boost::condition_variable cond;

  const std::vector<uint8_t> expected(test::GetRandomBytes());

  auto f = [&](const std::vector<uint8_t> & bytes) {
      ASSERT_EQ(expected, bytes);
      boost::mutex::scoped_lock lock(mutex);
      cond.notify_one();
    };
  on_server_binary_ = f; 

  client_.SendBinary(conn_, expected);

  const boost::chrono::seconds d(10);
  boost::mutex::scoped_lock lock(mutex);
  const auto timeout = cond.wait_for(lock, d);
  ASSERT_EQ(boost::cv_status::no_timeout, timeout);
}

TEST_F(TestHttpClientTest, recv_binary) {

  boost::mutex mutex;
  boost::condition_variable cond;

  const std::vector<uint8_t> expected(test::GetRandomBytes());

  auto f = [&](struct mg_connection * conn, const std::vector<uint8_t> & bytes) {
      ASSERT_EQ(conn_, conn);
      ASSERT_EQ(expected, bytes);
      boost::mutex::scoped_lock lock(mutex);
      cond.notify_one();
    };
  client_.on_binary_ = f;

  SendBinary(expected);

  const boost::chrono::seconds d(10);
  boost::mutex::scoped_lock lock(mutex);
  const auto timeout = cond.wait_for(lock, d);
  ASSERT_EQ(boost::cv_status::no_timeout, timeout);
}


}  // namespace server
}  // namespace ygg
