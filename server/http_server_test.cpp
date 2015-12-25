#include <gmock/gmock.h>
#include "http_server.h"


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


TEST_F(HttpServerTest, test) {

  HttpServer s;
  s.Start();
  s.Stop();
}


}  // namespace server
}  // namespace ygg
