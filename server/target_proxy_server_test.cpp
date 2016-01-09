#include <gmock/gmock.h>
#include "target_proxy_server.h"
#include "test/http_client.h"
#include "test/vars.h"
#include <mongoose/mongoose.h>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <atomic>


namespace ygg {
namespace server {


class TargetProxyServerTest : public ::testing::Test {
 protected:
    void SetUp() {
    }

    void TearDown() {
    }


 protected:
  TestHttpClient client_;

 private:
};


TEST_F(TargetProxyServerTest, test) {

  TargetProxyServer server;
  server.Start();

  struct mg_connection * ctrl = client_.Connect("127.0.0.1:8000", "/ctrl");
  struct mg_connection * target = client_.Connect("127.0.0.1:8000", "/target");
  ASSERT_NE(nullptr, ctrl);
  ASSERT_NE(nullptr, target);

  boost::mutex mutex;
  boost::condition_variable cond;

  client_.on_text_ = [&](struct mg_connection * conn, const std::string & text) {
      ASSERT_EQ("A", text);
      boost::mutex::scoped_lock lock(mutex);
      cond.notify_one();
    };

  client_.SendText(target, test::GetTargetJson("B"));
  {
    const boost::chrono::seconds d(5);
    boost::mutex::scoped_lock lock(mutex);
    cond.wait_for(lock, d);
  }
  client_.SendText(ctrl, test::GetCtrlJson("A", "B"));
  {
    const boost::chrono::seconds d(5);
    boost::mutex::scoped_lock lock(mutex);
    cond.wait_for(lock, d);
  }
  client_.SendText(target, "A");

  {
    const boost::chrono::seconds d(10);
    boost::mutex::scoped_lock lock(mutex);
    const auto timeout = cond.wait_for(lock, d);
    ASSERT_EQ(boost::cv_status::no_timeout, timeout);
  }

  client_.on_text_ = [](struct mg_connection*, const std::string&) {};
  
  client_.Close(ctrl);
  client_.Close(target);

  server.Stop();
}


}  // namespace server
}  // namespace ygg
