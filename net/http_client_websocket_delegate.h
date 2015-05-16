#ifndef YGG_NET_HTTP_CLIENT_WEBSOCKET_DELEGATE_H_
#define YGG_NET_HTTP_CLIENT_WEBSOCKET_DELEGATE_H_

#include <boost/system/error_code.hpp>


namespace net {
namespace http {
namespace client {
namespace websocket {


class Message;

class Delegate {
public:
	virtual void OnOpen() = 0;
	virtual void OnMessage(const Message * msg) = 0;
	virtual void OnClose() = 0;
	virtual void OnError(const boost::system::error_code & err) = 0;


protected:
	virtual ~Delegate() {}
};


}  // namespace websocket
}  // namespace client
}  // namespace http
}  // namespace net


#endif  // YGG_NET_HTTP_CLIENT_WEBSOCKET_DELEGATE_H_
