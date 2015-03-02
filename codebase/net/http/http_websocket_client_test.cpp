#include "gmock/gmock.h"
#include "codebase/net/http/http_websocket_client_connection.h"
#include "codebase/net/http/http_websocket_client_delegate.h"
#include <mongoose.h>
#include <boost/thread.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>


using ::testing::_;
using ::testing::Invoke;


class mongoose_delegate {
public:
	int ev_handler(struct mg_connection *conn, enum mg_event ev)  {

		if (ev == MG_REQUEST) {
			if (conn->is_websocket) {

			}
		}
		if (ev == MG_WS_CONNECT) {

		}
		if (ev == MG_CLOSE) {

		}
		return MG_FALSE;
	}

	MOCK_METHOD0(OnConnect, void());
	MOCK_METHOD1(OnMessage, void(int));
	MOCK_METHOD1(OnClose,   void(int));
};

class codebase_http_client_test : public ::testing::Test {
public:
	void BindDelegate(mongoose_delegate *delegate) {
		delegate_ = delegate;
	}

protected:
	virtual void SetUp() {

		stop_ = false;
		running_ = false;

		server_ = mg_create_server(this, &codebase_http_client_test::ev_handler);
		mg_set_option(server_, "listening_port", kPort_);

		thread_.swap(boost::thread([this]() {

			{
				std::lock_guard<std::mutex> lock(mutex_);
				running_ = true;
			}
			cv_.notify_one();

			while (!stop_) {
				mg_poll_server(server_, 1000);
			}
			mg_destroy_server(&server_);
			running_ = false;

		}));

		std::unique_lock<std::mutex> lock(mutex_);
		cv_.wait(lock, [this]() { return (bool)running_; });
	}

	virtual void TearDown() {
		stop_ = true;
		thread_.join();
		mg_destroy_server(&server_);
	}


private:
	static int ev_handler(struct mg_connection *conn, enum mg_event ev) {

		codebase_http_client_test *self = static_cast<codebase_http_client_test*>(conn->server_param);
		mongoose_delegate *delegate = self->delegate_;
		return delegate->ev_handler(conn, ev);
	}


protected:
	static const char *const kPort_;
	struct mg_server *server_;

	mongoose_delegate *delegate_;

	std::mutex mutex_;
	std::condition_variable cv_;
	std::atomic_bool running_;
	std::atomic_bool stop_;
	boost::thread thread_;
};

const char *const codebase_http_client_test::kPort_ = "80";



TEST_F(codebase_http_client_test, running_status_validation_test) {
	
	mongoose_delegate mock;
	BindDelegate(&mock);

	EXPECT_CALL(mock, OnConnect());
	EXPECT_CALL(mock, OnMessage(_));
	EXPECT_CALL(mock, OnClose(_));

	mock.OnConnect();
	mock.OnMessage(1);
	mock.OnClose(1);
}