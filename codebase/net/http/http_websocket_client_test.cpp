#include "gmock/gmock.h"
#include "codebase/net/http/http_websocket_client_connection.h"
#include "codebase/net/http/http_websocket_client_delegate.h"
#include <mongoose.h>
#include <boost/asio.hpp>
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
				OnMessage(1);
				return MG_TRUE;
			}
		}
		if (ev == MG_WS_CONNECT) {
			OnConnect();
			return MG_TRUE;
		}
		if (ev == MG_CLOSE) {
			OnClose(1);
			return MG_TRUE;
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



TEST_F(codebase_http_client_test, DISABLED_websocket_raw_connection_with_tcp_socket) {
	
	mongoose_delegate mock;
	BindDelegate(&mock);

	EXPECT_CALL(mock, OnConnect());
	EXPECT_CALL(mock, OnMessage(_));
	EXPECT_CALL(mock, OnClose(_));

	//mock.OnConnect();
	//mock.OnMessage(1);
	//mock.OnClose(1);





	const std::string url("ws://localhost:80/box");
	const std::string origin("http://localhost:80");

	ASSERT_LT(url.length(), 128);
	ASSERT_LT(origin.length(), 200);



	char host[128];
	int port;
	char path[128];

	if (false) {}
	else if (sscanf(url.c_str(), "ws://%[^:/]:%d/%s", host, &port, path) == 3) {
	}
	else if (sscanf(url.c_str(), "ws://%[^:/]/%s", host, path) == 2) {
		port = 80;
	}
	else if (sscanf(url.c_str(), "ws://%[^:/]:%d", host, &port) == 2) {
		path[0] = '\0';
	}
	else if (sscanf(url.c_str(), "ws://%[^:/]", host) == 1) {
		port = 80;
		path[0] = '\0';
	}
	else {
		ASSERT_TRUE(false);
	}



	char sport[16];
	sprintf(sport, "%d", port);



	boost::asio::io_service io_service;

	typedef boost::asio::ip::tcp tcp;

	// Get a list of endpoints corresponding to the server name.
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(host, sport);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	//tcp::resolver::iterator endpoint_iterator = resolver.resolve({ "localhost", "80" });
	
	

	// Try each endpoint until we successfully establish a connection.
	tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);

	// Form the request. We specify the "Connection: close" header so that the
	// server will close the socket after transmitting the response. This will
	// allow us to treat all data up until the EOF as the content.
	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	request_stream << "GET /" << path << " HTTP/1.1\r\n";
	request_stream << "Host: " << host << "\r\n";
	request_stream << "Upgrade: websocket\r\n";
	request_stream << "Connection: Upgrade\r\n";
	if (!origin.empty()) {
		request_stream << "Origin: " << origin << "\r\n";
	}
	request_stream << "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n";
	request_stream << "Sec-WebSocket-Version: 13\r\n";
	request_stream << "\r\n";

	// Send the request.
	boost::asio::write(socket, request);

	// Read the response status line. The response streambuf will automatically
	// grow to accommodate the entire line. The growth may be limited by passing
	// a maximum size to the streambuf constructor.
	boost::asio::streambuf response;
	boost::asio::read_until(socket, response, "\r\n");

	// Check that response is OK.
	std::istream response_stream(&response);
	std::string http_version;
	response_stream >> http_version;
	unsigned int status_code;
	response_stream >> status_code;
	std::string status_message;
	std::getline(response_stream, status_message);
	if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
		std::cout << "Invalid response\n";
		ASSERT_TRUE(false);
	}
	if (status_code != 101) {
		std::cout << "Response returned with status code " << status_code << "\n";
		ASSERT_TRUE(false);
	}

	// Read the response headers, which are terminated by a blank line.
	boost::asio::read_until(socket, response, "\r\n\r\n");

	// Process the response headers.
	std::string header;
	while (std::getline(response_stream, header) && header != "\r")
		std::cout << header << "\n";
	std::cout << "\n";


	socket.close();
}

TEST_F(codebase_http_client_test, websocket_client_connection) {

	mongoose_delegate mock;
	BindDelegate(&mock);

	EXPECT_CALL(mock, OnConnect());
	EXPECT_CALL(mock, OnMessage(_));
	EXPECT_CALL(mock, OnClose(_));



	typedef codebase::net::http::websocket::client::Connection WebSocket;
	auto ptr(WebSocket::New("", nullptr));
}