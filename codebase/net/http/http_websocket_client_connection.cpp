#include "http_websocket_client_connection.h"


void intrusive_ptr_add_ref(codebase::net::http::websocket::client::Connection *conn) {
	conn->add_ref();
}

void intrusive_ptr_release(codebase::net::http::websocket::client::Connection *conn) {
	conn->release();
}




namespace codebase {
namespace net {
namespace http {
namespace websocket {
namespace client {


Connection::Ptr Connection::New(const std::string& url, Delegate* delegate) {


	return Ptr(new Connection(url, delegate));
}

void Connection::Send(const Message *msg) {
	
}

void Connection::Close() {

}

const std::string& Connection::url() const {
	return url_;
}

Connection::ReadyState Connection::readyState() const {
	return readyState_;
}

Connection::Connection(const std::string& url, Delegate* delegate)
	: url_(url)
	, readyState_(CLOSED)
	, delegate_(delegate)
	, resolver_(io_service_)
	, socket_(io_service_)
	, thread_(boost::bind(&boost::asio::io_service::run, &io_service_)) {
	


	char *path = "box";
	char *host = "localhost";



	std::ostream request_stream(&request_);
	request_stream << "GET /" << path << " HTTP/1.1\r\n";
	request_stream << "Host: " << host << "\r\n";
	request_stream << "Upgrade: websocket\r\n";
	request_stream << "Connection: Upgrade\r\n";
	//if (!origin.empty()) {
	//	request_stream << "Origin: " << origin << "\r\n";
	//}
	request_stream << "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n";
	request_stream << "Sec-WebSocket-Version: 13\r\n";
	request_stream << "\r\n";

	boost::asio::ip::tcp::resolver::query query(host, "http");
	resolver_.async_resolve(query,
		boost::bind(&Connection::handle_resolve, this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::iterator));
}



void Connection::handle_resolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
	if (!err)
	{
		// Attempt a connection to each endpoint in the list until we
		// successfully establish a connection.
		boost::asio::async_connect(socket_, endpoint_iterator,
			boost::bind(&Connection::handle_connect, this,
			boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err.message() << "\n";
	}
}

void Connection::handle_connect(const boost::system::error_code& err)
{
	if (!err)
	{
		// The connection was successful. Send the request.
		boost::asio::async_write(socket_, request_,
			boost::bind(&Connection::handle_write_request, this,
			boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err.message() << "\n";
	}
}

void Connection::handle_write_request(const boost::system::error_code& err)
{
	if (!err)
	{
		// Read the response status line. The response_ streambuf will
		// automatically grow to accommodate the entire line. The growth may be
		// limited by passing a maximum size to the streambuf constructor.
		boost::asio::async_read_until(socket_, response_, "\r\n",
			boost::bind(&Connection::handle_read_status_line, this,
			boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err.message() << "\n";
	}
}

void Connection::handle_read_status_line(const boost::system::error_code& err)
{
	if (!err)
	{
		// Check that response is OK.
		std::istream response_stream(&response_);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			std::cout << "Invalid response\n";
			return;
		}
		if (status_code != 200)
		{
			std::cout << "Response returned with status code ";
			std::cout << status_code << "\n";
			return;
		}

		// Read the response headers, which are terminated by a blank line.
		boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
			boost::bind(&Connection::handle_read_headers, this,
			boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err << "\n";
	}
}

void Connection::handle_read_headers(const boost::system::error_code& err)
{
	if (!err)
	{
		// Process the response headers.
		std::istream response_stream(&response_);
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
			std::cout << header << "\n";
		std::cout << "\n";

		// Write whatever content we already have to output.
		if (response_.size() > 0)
			std::cout << &response_;

		// Start reading remaining data until EOF.
		boost::asio::async_read(socket_, response_,
			boost::asio::transfer_at_least(1),
			boost::bind(&Connection::handle_read_content, this,
			boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err << "\n";
	}
}

void Connection::handle_read_content(const boost::system::error_code& err)
{
	if (!err)
	{
		// Write all of the data that has been read so far.
		std::cout << &response_;

		// Continue reading remaining data until EOF.
		boost::asio::async_read(socket_, response_,
			boost::asio::transfer_at_least(1),
			boost::bind(&Connection::handle_read_content, this,
			boost::asio::placeholders::error));
	}
	else if (err != boost::asio::error::eof)
	{
		std::cout << "Error: " << err << "\n";
	}
}




}  // namespace client
}  // namespace websocket
}  // namespace http
}  // namespace net
}  // namespace codebase