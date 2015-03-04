#include "gmock/gmock.h"
#include "codebase/net/http/http_server.h"
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>


class codebase_http_server_test : public ::testing::Test {
protected:
	virtual void SetUp() {
		httpd_.Start(kPort);
	}

	virtual void TearDown() {
		httpd_.Stop();
	}

protected:
	static const int kPort = 80;
	codebase::HttpServer httpd_;
};


class WebSocketClient {
	typedef websocketpp::client<websocketpp::config::asio_client> Client;

public:
	WebSocketClient() {
		client_.init_asio();
		client_.set_message_handler(boost::bind(&WebSocketClient::on_message, &client_, _1, _2));
	}

	~WebSocketClient() {

	}

	void Connect(const std::string& uri) {
		websocketpp::lib::error_code ec;
		conn_ = client_.get_connection(uri + "/getCaseCount", ec);
		client_.connect(conn_);
		client_.run();
	}

	void Close() {

		client_.stop();

		websocketpp::lib::error_code ec;
		client_.close(conn_->get_handle(), websocketpp::close::status::going_away, "", ec);
	}


private:

	static void on_message(Client* c, websocketpp::connection_hdl hdl, Client::message_ptr msg) {
		
	}


private:
	Client::connection_ptr conn_;
	Client client_;
};


TEST_F(codebase_http_server_test, running_status_validation_test) {
	codebase::HttpServer httpd;
	httpd.Start(81);
	ASSERT_TRUE(httpd.IsRunning());
	httpd.Stop();
	ASSERT_TRUE(!httpd.IsRunning());
}

TEST_F(codebase_http_server_test, global_server_is_running) {
	ASSERT_TRUE(httpd_.IsRunning());
}

TEST_F(codebase_http_server_test, DISABLED_websocket_client_connect_and_close) {
	ASSERT_TRUE(httpd_.IsRunning());

	WebSocketClient c;
	c.Connect("ws://localhost/");
	
}