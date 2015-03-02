#ifndef YGG_CODEBASE_NET_HTTP_WEBSOCKET_CLIENT_DELEGATE_H_
#define YGG_CODEBASE_NET_HTTP_WEBSOCKET_CLIENT_DELEGATE_H_

#include "codebase/object2.h"


namespace codebase {
namespace net {
namespace http {
namespace websocket {
namespace client {


class Message;

class Delegate {
public:
	virtual void OnOpen() = 0;
	virtual void OnMessage(const Message *msg) = 0;
	virtual void OnClose() = 0;
	virtual void OnError() = 0;


protected:
	virtual ~Delegate() {}
};


}  // namespace client
}  // namespace websocket
}  // namespace http
}  // namespace net
}  // namespace codebase


#endif  // YGG_CODEBASE_NET_HTTP_WEBSOCKET_CLIENT_DELEGATE_H_