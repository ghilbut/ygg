#include <gmock/gmock.h>
#include "http_server.h"


using ::testing::_;


namespace ygg {
namespace server {


class HttpServerTest : public ::testing::Test {
 public:
  void SetUp() {
  }

  void TearDown() {
  }


 protected:

};


class MockHttpServerDelegate : public HttpServer::Delegate {
 public:
  MOCK_METHOD2(OnRequest, void(struct mg_connection*, struct http_message*));
  MOCK_METHOD1(OnWSOpened, void(struct mg_connection*));
  MOCK_METHOD2(OnWSMessage, void(struct mg_connection*,
                                 struct websocket_message*));
  MOCK_METHOD1(OnWSClosed, void(struct mg_connection*));
};





static int s_exit_flag = 0;
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
  EXPECT_CALL(mock, OnRequest(_, _)).WillOnce(::testing::InvokeWithoutArgs(DoStop));

  HttpServer s;
  s.BindDelegate(&mock);
  s.Start();





  struct mg_mgr mgr;
  mg_mgr_init(&mgr, NULL);

  mg_connect_http(&mgr, ev_handler, "http://127.0.0.1:8000/B/methods", NULL, NULL);
  while (s_exit_flag == 0) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);





  s.Stop();
}




void DoUpdate() {
  ++s_exit_flag;
}

static void cb4(struct mg_connection * nc, int ev, void *ev_data) {
  struct websocket_message *wm = (struct websocket_message *) ev_data;

  if (ev == MG_EV_WEBSOCKET_FRAME) {
    memcpy(nc->user_data, wm->data, wm->size);
    mg_send_websocket_frame(nc, WEBSOCKET_OP_CLOSE, NULL, 0);
  } else if (ev == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
    /* Send "hi" to server. server must reply "A". */
    struct mg_str h[2];
    h[0].p = "h";
    h[0].len = 1;
    h[1].p = "i";
    h[1].len = 1;
    mg_send_websocket_framev(nc, WEBSOCKET_OP_TEXT, h, 2);
  }
}

TEST_F(HttpServerTest, test2) {

  MockHttpServerDelegate mock;
  EXPECT_CALL(mock, OnWSOpened(_)).WillOnce(::testing::InvokeWithoutArgs(DoUpdate));
  EXPECT_CALL(mock, OnWSMessage(_, _)).WillOnce(::testing::InvokeWithoutArgs(DoUpdate));
  EXPECT_CALL(mock, OnWSClosed(_)).WillOnce(::testing::InvokeWithoutArgs(DoUpdate));

  HttpServer s;
  s.BindDelegate(&mock);
  s.Start();





  s_exit_flag = 0;

  struct mg_mgr mgr;
  struct mg_connection *nc;
  const char *local_addr = "127.0.0.1:8000";
  char buf[20] = "";

  mg_mgr_init(&mgr, NULL);
  nc = mg_connect(&mgr, local_addr, cb4);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = buf;
  mg_send_websocket_handshake(nc, "/ws", NULL);
  while (s_exit_flag != 3) {
    mg_mgr_poll(&mgr, 1000);
    
    if (s_exit_flag == 2) {
      mg_send_websocket_frame(nc, WEBSOCKET_OP_CLOSE, NULL, 0);
    }
  }
  
  mg_mgr_free(&mgr);





  s.Stop();
}






}  // namespace server
}  // namespace ygg
