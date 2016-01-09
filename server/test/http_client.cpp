#include "http_client.h"
#include <cassert>


namespace ygg {
namespace server {


TestHttpClient::TestHttpClient() 
  : stop_(false) {

  std::atomic_bool is_run(false);

  on_text_ = [](struct mg_connection*, const std::string & text) {};
  on_binary_ = [](struct mg_connection*, const std::vector<uint8_t> & bytes) {};
  on_closed_ = [](struct mg_connection*) {};

  mg_mgr_init(&mgr_, this);

  boost::mutex mutex;
  boost::condition_variable cond;

  boost::thread t([&]() {

      mutex.lock();
      is_run = true;
      cond.notify_one();
      mutex.unlock();

      while(!stop_) {
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
}

TestHttpClient::~TestHttpClient() {
  stop_ = true;
  thread_.join();
}

struct mg_connection * TestHttpClient::Connect(const std::string & address,
                                               const std::string & uri) {

  boost::mutex mutex;
  boost::condition_variable cond;

  struct mg_connection * conn;
  on_connected_ = [&](struct mg_connection * ws) {
      assert(conn == ws);
      boost::mutex::scoped_lock lock(mutex);
      cond.notify_one();
    };

  conn = mg_connect(&mgr_, address.c_str(), callback);
  mg_set_protocol_http_websocket(conn);
  mg_send_websocket_handshake(conn, uri.c_str(), nullptr);

  const boost::chrono::seconds d(10);
  boost::mutex::scoped_lock lock(mutex);
  const auto timeout = cond.wait_for(lock, d);
  if (boost::cv_status::timeout == timeout) {
    mg_send_websocket_frame(conn, WEBSOCKET_OP_CLOSE, nullptr, 0);
    return nullptr;
  }

  return conn;
}

void TestHttpClient::SendText(struct mg_connection * conn,
                              const std::string & text) {
  mg_send_websocket_frame(conn,
                          WEBSOCKET_OP_TEXT,
                          text.c_str(),
                          text.length());
}

void TestHttpClient::SendBinary(struct mg_connection * conn,
                                const std::vector<uint8_t> & bytes) {
  mg_send_websocket_frame(conn,
                          WEBSOCKET_OP_BINARY,
                          &bytes[0],
                          bytes.size());
}

void TestHttpClient::Close(struct mg_connection * conn) {

  boost::mutex mutex;
  boost::condition_variable cond;

  on_closed_ = [&](struct mg_connection * ws) {
      assert(conn == ws);
      boost::mutex::scoped_lock lock(mutex);
      cond.notify_one();
    };

  mg_send_websocket_frame(conn, WEBSOCKET_OP_CLOSE, nullptr, 0);
  boost::mutex::scoped_lock lock(mutex);
  cond.wait(lock);
}

void TestHttpClient::callback(struct mg_connection * conn,
                              int event,
                              void * data) {
  auto d = conn->mgr->user_data;
  auto p = reinterpret_cast<TestHttpClient*>(d);
  p->DoCallback(conn, event, data);
}

void TestHttpClient::DoCallback(struct mg_connection * conn,
                                int event,
                                void * data) {
  if (event == MG_EV_CONNECT) {
    const int err = *reinterpret_cast<int*>(data);
    if (err != 0) {
      fprintf(stderr, "connect() failed: %s\n", strerror(err));
    }
    return;
  }

  if (event == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
    on_connected_(conn);
    return;
  }

  if (event == MG_EV_WEBSOCKET_FRAME) {
    const auto wm = reinterpret_cast<struct websocket_message*>(data);

    if ((wm->flags & WEBSOCKET_OP_TEXT) == WEBSOCKET_OP_TEXT) {
      const std::string text(wm->data, wm->data + wm->size);
      on_text_(conn, text);
      return;
    }

    if ((wm->flags & WEBSOCKET_OP_BINARY) == WEBSOCKET_OP_BINARY) {
      const std::vector<uint8_t> bytes(wm->data, wm->data + wm->size);
      on_binary_(conn, bytes);
      return;
    }
  }

  if (event == MG_EV_CLOSE) {
    if ((conn->flags & MG_F_IS_WEBSOCKET) == MG_F_IS_WEBSOCKET) {
      on_closed_(conn);
    }
    return;
  }
}


}  // namespace server
}  // namespace ygg
