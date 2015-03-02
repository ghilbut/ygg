#include "http_websocket_client_connection.h"


namespace codebase {
namespace net {
namespace http {
namespace websocket {
namespace client {


void intrusive_ptr_add_ref(Connection *conn) {
	conn->add_ref();
}

void intrusive_ptr_release(Connection *conn) {
	conn->release();
}


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
	, delegate_(delegate) {
	// nothing
}


}  // namespace client
}  // namespace websocket
}  // namespace http
}  // namespace net
}  // namespace codebase