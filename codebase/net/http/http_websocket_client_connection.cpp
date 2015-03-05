#include "http_websocket_client_connection.h"
#include "http_websocket_client_delegate.h"


namespace codebase {
namespace net {
namespace http {
namespace websocket {
namespace client {


Connection::Ptr Connection::New(boost::asio::io_service &io_service, const std::string& url, Delegate* delegate) {
	return Ptr(new Connection(io_service, url, delegate));
}

void Connection::Send(const Message *msg) {

	if (readyState_ != OPEN) {
		return;
	}	
}

void Connection::Close(const boost::system::error_code& err) {

	if (readyState_ == CLOSING || readyState_ == CLOSED) {
		return;
	}
	readyState_ = CLOSING;
	io_service_.post(boost::bind(&Connection::handle_close, this, err));
}

const std::string& Connection::url() const {
	return url_;
}

Connection::ReadyState Connection::readyState() const {
	return readyState_;
}

Connection::Connection(boost::asio::io_service &io_service, const std::string& url, Delegate* delegate)
	: url_(url)
	, readyState_(CONNECTING)
	, delegate_(delegate)
	, io_service_(io_service)
	, resolver_(io_service)
	, socket_(io_service) {
	


	char *path = "box";
	char *host = "localhost";



	std::ostream request_stream(&request_);
	request_stream << "GET /" << path << " HTTP/1.1\r\n";
	request_stream << "Host: " << host << "\r\n";
	request_stream << "Upgrade: websocket\r\n";
	request_stream << "Connection: Upgrade\r\n";
	request_stream << "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n";
	//if (!origin.empty()) {
	//	request_stream << "Origin: " << origin << "\r\n";
	//}
	//request_stream << "Sec-WebSocket-Extentions: ";
	//request_stream << "Sec-WebSocket-Protocol: chat, superchat";
	request_stream << "Sec-WebSocket-Version: 13\r\n";
	request_stream << "\r\n";

	boost::asio::ip::tcp::resolver::query query(host, "http");
	resolver_.async_resolve(query,
		boost::bind(&Connection::handle_resolve, this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::iterator));
}



void Connection::handle_resolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {

	if (err) {
		Close(err);
	}
	
	// Attempt a connection to each endpoint in the list until we
	// successfully establish a connection.
	boost::asio::async_connect(socket_, endpoint_iterator,
		boost::bind(&Connection::handle_connect, this,
		boost::asio::placeholders::error));
}

void Connection::handle_connect(const boost::system::error_code& err) {

	if (err) {
		Close(err);
	}

	boost::asio::async_write(socket_, request_,
		boost::bind(&Connection::handle_handshake_request, this,
		boost::asio::placeholders::error));
}

void Connection::handle_handshake_request(const boost::system::error_code& err) {

	if (err) {
		Close(err);
	}

	// Read the response status line. The response_ streambuf will
	// automatically grow to accommodate the entire line. The growth may be
	// limited by passing a maximum size to the streambuf constructor.
	boost::asio::async_read_until(socket_, response_, "\r\n",
		boost::bind(&Connection::handle_handshake_response_status, this,
		boost::asio::placeholders::error));
}

void Connection::handle_handshake_response_status(const boost::system::error_code& err) {

	if (err) {
		Close(err);
	}

	readyState_ = OPEN;
	io_service_.dispatch(boost::bind(&Delegate::OnOpen, delegate_));

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
	if (status_code != 101)
	{
		std::cout << "Response returned with status code ";
		std::cout << status_code << "\n";
		return;
	}

	// Read the response headers, which are terminated by a blank line.
	boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
		boost::bind(&Connection::handle_handshake_response_headers, this,
		boost::asio::placeholders::error));
}

void Connection::handle_handshake_response_headers(const boost::system::error_code& err) {

	if (err) {
		Close(err);
	}

	// Sec-WebSocket-Accept
	// Sec-WebSocket-Protocol
	// Sec-WebSocket-Version (error case)

	// Process the response headers.
	char key[128];
	char value[128];
	std::string header;
	std::istream response_stream(&response_);
	while (std::getline(response_stream, header) && header != "\r") {
		if (sscanf(header.c_str(), "%[^:]: %[^\r\n]", key, value) == 2) {
			std::cout << key << ": " << value << "\n";
		}
	}
	std::cout << "\n";

	// Write whatever content we already have to output.
	if (response_.size() > 0)
		std::cout << &response_;

	// Start reading remaining data until EOF.
	boost::asio::async_read(socket_, response_,
		boost::asio::transfer_at_least(1),
		boost::bind(&Connection::handle_recv_message_headers, this,
		boost::asio::placeholders::error));
}

// http://tools.ietf.org/html/rfc6455#section-5.2  Base Framing Protocol
//
//  0                   1                   2                   3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-------+-+-------------+-------------------------------+
// |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
// |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
// |N|V|V|V|       |S|             |   (if payload len==126/127)   |
// | |1|2|3|       |K|             |                               |
// +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
// |     Extended payload length continued, if payload len == 127  |
// + - - - - - - - - - - - - - - - +-------------------------------+
// |                               |Masking-key, if MASK set to 1  |
// +-------------------------------+-------------------------------+
// | Masking-key (continued)       |          Payload Data         |
// +-------------------------------- - - - - - - - - - - - - - - - +
// :                     Payload Data continued ...                :
// + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
// |                     Payload Data continued ...                |
// +---------------------------------------------------------------+

void Connection::handle_recv_message_headers(const boost::system::error_code& err)
{
	if (err) {
		Close(err);
	}

	// Write all of the data that has been read so far.
	std::cout << &response_;

	// Continue reading remaining data until EOF.
	boost::asio::async_read(socket_, response_,
		boost::asio::transfer_at_least(1),
		boost::bind(&Connection::handle_recv_message_payload, this,
		boost::asio::placeholders::error));
}

void Connection::handle_recv_message_payload(const boost::system::error_code& err)
{
	if (err) {
		Close(err);
	}

	// Write all of the data that has been read so far.
	std::cout << &response_;

	// Continue reading remaining data until EOF.
	boost::asio::async_read(socket_, response_,
		boost::asio::transfer_at_least(1),
		boost::bind(&Connection::handle_recv_message_headers, this,
		boost::asio::placeholders::error));
}

void Connection::handle_send_message(const boost::system::error_code& err) {

}

void Connection::handle_close(const boost::system::error_code& err) {

	if (err) {
		std::cout << "Error: " << err.message() << "\n";
		delegate_->OnError(err);
	}

	readyState_ = CLOSED;
	socket_.close();
	delegate_->OnClose();
}


}  // namespace client
}  // namespace websocket
}  // namespace http
}  // namespace net
}  // namespace codebase


void intrusive_ptr_add_ref(codebase::net::http::websocket::client::Connection *conn) {
	conn->add_ref();
}

void intrusive_ptr_release(codebase::net::http::websocket::client::Connection *conn) {
	conn->release();
}