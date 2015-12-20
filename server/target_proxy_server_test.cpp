#include <gmock/gmock.h>
#include "target_proxy_server.h"
#include <mongoose/mongoose.h>
#include <boost/bind.hpp>
#include <boost/thread.hpp>


namespace ygg {
namespace server {


class TargetProxyServerTest : public ::testing::Test {
public:
    void SetUp() {

        mg_mgr_init(&mgr_, NULL);
        nc_ = mg_bind(&mgr_, "8000", ev_handler);
        mg_set_protocol_http_websocket(nc_);

        stop_ = false;
        auto p = boost::bind(&TargetProxyServerTest::polling, this);
        boost::thread t(p);
        thread_.swap(t);

        starting_ = false;
        boost::mutex::scoped_lock lock(mutex_);
        while (!starting_) {
            cond_.wait(lock);
        }
    }

    void TearDown() {
        stop_ = true;
        thread_.join();
        mg_mgr_free(&mgr_);
    }


private:
    void polling() {

        mutex_.lock();
        starting_ = true;
        cond_.notify_all();
        mutex_.unlock();

        while (!stop_) {
            mg_mgr_poll(&mgr_, 1000);
        }
    }

    static void ev_handler(struct mg_connection *nc, int ev, void *p) {
        if (ev == MG_EV_HTTP_REQUEST) {
        }
    }

protected:

private:
    struct mg_mgr mgr_;
    struct mg_connection * nc_;

    boost::thread thread_;
    boost::mutex mutex_;
    boost::condition_variable cond_;
    bool starting_;

    std::atomic_bool stop_;
};


TEST_F(TargetProxyServerTest, test) {
}


}  // namespace server
}  // namespace ygg
