#include <gmock/gmock.h>
#include "target_proxy_server.h"
#include "test/http_client.h"
#include "test/vars.h"
#include <json/json.h>
#include <mongoose/mongoose.h>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <atomic>
#include <future>


namespace ygg {
namespace server {


class TargetProxyServerTest : public ::testing::Test {
 protected:
    void SetUp() {
      server.Start();
      ctrl_ = client_.Connect("127.0.0.1:8000", "/ctrl");
      target_ = client_.Connect("127.0.0.1:8000", "/target");

      ASSERT_NE(nullptr, ctrl_);
      ASSERT_NE(nullptr, target_);

      boost::mutex mutex;
      boost::condition_variable cond;

      client_.on_text_ = [&](struct mg_connection * conn,
                             const std::string & text) {
          Json::Value root;
          Json::Reader reader;
          ASSERT_TRUE(reader.parse(text, root));
          ASSERT_TRUE(root["endpoint"].isString());
          ASSERT_EQ("B", root["endpoint"].asString());
          boost::mutex::scoped_lock lock(mutex);
          cond.notify_one();
        };
      client_.SendText(target_, test::GetTargetJson("B"));
      {
        const boost::chrono::seconds d(10);
        boost::mutex::scoped_lock lock(mutex);
        const auto timeout = cond.wait_for(lock, d);
        ASSERT_EQ(boost::cv_status::no_timeout, timeout);
      }

      client_.on_text_ = [&](struct mg_connection * conn,
                             const std::string & text) {
          Json::Value root;
          Json::Reader reader;
          ASSERT_TRUE(reader.parse(text, root));
          ASSERT_TRUE(root["user"].isString());
          ASSERT_EQ("A", root["user"].asString());
          ASSERT_TRUE(root["endpoint"].isString());
          ASSERT_EQ("B", root["endpoint"].asString());
          boost::mutex::scoped_lock lock(mutex);
          cond.notify_one();
        };
      client_.SendText(ctrl_, test::GetCtrlJson("A", "B"));
      {
        const boost::chrono::seconds d(10);
        boost::mutex::scoped_lock lock(mutex);
        const auto timeout = cond.wait_for(lock, d);
        ASSERT_EQ(boost::cv_status::no_timeout, timeout);
      }

      client_.on_text_ = [&](struct mg_connection * conn,
                             const std::string & text) {
          Json::Value root;
          Json::Reader reader;
          ASSERT_EQ("A", text);
          boost::mutex::scoped_lock lock(mutex);
          cond.notify_one();
        };
      client_.SendText(target_, "A");
      {
        const boost::chrono::seconds d(10);
        boost::mutex::scoped_lock lock(mutex);
        const auto timeout = cond.wait_for(lock, d);
        ASSERT_EQ(boost::cv_status::no_timeout, timeout);
      }
    }

    void TearDown() {
      client_.Close(ctrl_);
      client_.Close(target_);
      server.Stop();
    }


 protected:
  TestHttpClient client_;
  struct mg_connection * ctrl_;
  struct mg_connection * target_;

 private:
  TargetProxyServer server;

};


TEST_F(TargetProxyServerTest, test) {

}


}  // namespace server
}  // namespace ygg
