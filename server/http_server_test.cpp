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





TEST_F(HttpServerTest, test) {

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


}  // namespace server
}  // namespace ygg
