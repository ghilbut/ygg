#ifndef YGG_CODEBASE_NET_HTTP_WEBSOCKET_CLIENT_CONNECTION_H_
#define YGG_CODEBASE_NET_HTTP_WEBSOCKET_CLIENT_CONNECTION_H_

#include "codebase/object2.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>


namespace codebase {
namespace net {
namespace http {
namespace websocket {
namespace client {


class Delegate;
class Message;


class Connection : public Object2 {
public:
	typedef enum {
		CONNECTING = 0,
		OPEN,
		CLOSING,
		CLOSED
	} ReadyState;

	typedef boost::intrusive_ptr<Connection> Ptr;
	static Ptr New(boost::asio::io_service &io_service, const std::string& url, Delegate* delegate);

	// method
	void Send(const Message *msg);
	void Close();

	// property
	const std::string& url() const;
	ReadyState readyState() const;

private:
	Connection(boost::asio::io_service &io_service, const std::string& url, Delegate* delegate);
	~Connection() {}

	void handle_resolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
	void handle_connect(const boost::system::error_code& err);
	void handle_write_request(const boost::system::error_code& err);
	void handle_read_status_line(const boost::system::error_code& err);
	void handle_read_headers(const boost::system::error_code& err);
	void handle_read_content(const boost::system::error_code& err);

	void handle_close(const boost::system::error_code& err);


private:
	const std::string url_;
	std::atomic<ReadyState> readyState_;
	Delegate*  delegate_;



	
	boost::asio::io_service &io_service_;
	boost::asio::ip::tcp::resolver resolver_;
	boost::asio::ip::tcp::socket socket_;

	boost::asio::streambuf request_;
	boost::asio::streambuf response_;
};


}  // namespace client
}  // namespace websocket
}  // namespace http
}  // namespace net
}  // namespace codebase


void intrusive_ptr_add_ref(codebase::net::http::websocket::client::Connection *conn);
void intrusive_ptr_release(codebase::net::http::websocket::client::Connection *conn);


#endif  // YGG_CODEBASE_NET_HTTP_WEBSOCKET_CLIENT_CONNECTION_H_