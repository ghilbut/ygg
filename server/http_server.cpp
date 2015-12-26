#include "http_server.h"


namespace ygg {
namespace server {


static bool is_websocket(const struct mg_connection * conn) {
  return (conn->flags & MG_F_IS_WEBSOCKET != 0);
}


HttpServer::HttpServer() 
  : stop_(true)
  , status_(kStopped) {
  // nothing
}

HttpServer::~HttpServer() {
  // nothing
}

void HttpServer::BindDelegate(Delegate * delegate) {
  delegate_ = delegate;
}

void HttpServer::UnbindDelegate() {
  delegate_ = nullptr;
}

bool HttpServer::Start() {
  if (status_ == kStarting || status_ == kRunning) {
    return false;
  }

  if (status_ == kStopping || status_ == kStopped) {
    boost::mutex::scoped_lock lock(mutex_);
    while (status_ != kStopped) {
      stopped_cond_.wait(lock);
    }
  }

  stop_ = false;
  status_ = kStarting;

  mg_mgr_init(&mgr_, this);
  conn_ = mg_bind(&mgr_, "8000", ev_handler);
  mg_set_protocol_http_websocket(conn_);

  auto b = boost::bind(&HttpServer::polling, this);
  boost::thread t(b);
  thread_.swap(t);

  boost::mutex::scoped_lock lock(mutex_);
  while (status_ != kRunning) {
    running_cond_.wait(lock);
  }

  return true;
}

void HttpServer::Stop() {
  status_ = kStopping;
  stop_ = true;

  thread_.join();
}

void HttpServer::polling() {
  mutex_.lock();
  status_ = kRunning;
  running_cond_.notify_all();
  mutex_.unlock();

  while (!stop_) {
    mg_mgr_poll(&mgr_, 1000);
  }

  mg_mgr_free(&mgr_);

  mutex_.lock();
  status_ = kStopped;
  stopped_cond_.notify_all();
  mutex_.unlock();
}

void HttpServer::DoHandle(struct mg_connection * conn, int event, void * data) {

  switch (event) {
    case MG_EV_HTTP_REQUEST:
      delegate_->OnRequest(conn, (struct http_message*) data);
      break;
    case MG_EV_WEBSOCKET_HANDSHAKE_DONE:
      break;
    case MG_EV_WEBSOCKET_FRAME:
      break;
    case MG_EV_CLOSE:
      if (is_websocket(conn)) {
      }
      break;
    default:
      break;
  }
}

void HttpServer::ev_handler(struct mg_connection * conn,
                            int event,
                            void * data) {
  auto pThis = reinterpret_cast<HttpServer*>(conn->mgr->user_data);
  pThis->DoHandle(conn, event, data);
}


}  // namespace server
}  // namespace ygg
