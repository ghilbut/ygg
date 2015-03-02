#ifndef YGG_CODEBASE_NET_HTTP_WEBSOCKET_CLIENT_CONNECTION_H_
#define YGG_CODEBASE_NET_HTTP_WEBSOCKET_CLIENT_CONNECTION_H_

#include "codebase/object2.h"
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
	Ptr New(const std::string& url, Delegate* delegate);

	// method
	void Send(const Message *msg);
	void Close();

	// property
	const std::string& url() const;
	ReadyState readyState() const;

private:
	Connection(const std::string& url, Delegate* delegate);
	~Connection() {}


private:
	const std::string url_;
	ReadyState readyState_;
	Delegate*  delegate_;
};


}  // namespace client
}  // namespace websocket
}  // namespace http
}  // namespace net
}  // namespace codebase


#endif  // YGG_CODEBASE_NET_HTTP_WEBSOCKET_CLIENT_CONNECTION_H_