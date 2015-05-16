#ifndef YGG_NET_HTTP_CLIENT_WEBSOCKET_CONNECTION_H_
#define YGG_NET_HTTP_CLIENT_WEBSOCKET_CONNECTION_H_

#include "codebase/object.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>


namespace net {
namespace http {
namespace client {
namespace websocket {


class Delegate;
class Message;


class Connection : public codebase::Object<Connection> {
public:
	typedef enum {
		CONNECTING = 0,
		OPEN,
		CLOSING,
		CLOSED
	} ReadyState;

	static Ptr New(boost::asio::io_service & io_service, const std::string & url, Delegate * delegate);

	// method
	void Send(const Message * msg);
	void Close(const boost::system::error_code & err = boost::system::error_code());

	// property
	const std::string & url() const;
	ReadyState readyState() const;

private:
	Connection(boost::asio::io_service & io_service, const std::string & url, Delegate * delegate);
	~Connection() {}

	void handle_resolve(const boost::system::error_code & err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
	void handle_connect(const boost::system::error_code & err);
	void handle_handshake_request(const boost::system::error_code & err);
	void handle_handshake_response_status(const boost::system::error_code & err);
	void handle_handshake_response_headers(const boost::system::error_code & err);
	void handle_recv_message_headers(const boost::system::error_code & err);
	void handle_recv_message_payload(const boost::system::error_code & err);
	void handle_send_message(const boost::system::error_code & err);

	void handle_close(const boost::system::error_code & err);


private:
	const std::string url_;
	std::atomic<ReadyState> readyState_;
	Delegate * delegate_;



	
	boost::asio::io_service & io_service_;
	boost::asio::ip::tcp::resolver resolver_;
	boost::asio::ip::tcp::socket socket_;

	boost::asio::streambuf request_;
	boost::asio::streambuf response_;
};


}  // namespace websocket
}  // namespace client
}  // namespace http
}  // namespace net



#endif  // YGG_NET_HTTP_CLIENT_WEBSOCKET_CONNECTION_H_
