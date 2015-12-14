#ifndef YGG_LIBUSER_SERVER_CONNECTOR_H_
#define YGG_LIBUSER_SERVER_CONNECTOR_H_

#include "codebase/connection.h"
#include <string>


namespace user {
namespace server {


class Connector {
public:
    class Delegate {
    public:
        virtual void OnConnected(const std::string & user, Connection::Ptr conn) = 0;
        virtual void OnError(const std::string & user) = 0;
    };

public:
    virtual void Connect(const std::string & user, const std::string & endpoint) = 0;

    void BindDelegate(Delegate * delegate) {
        delegate_ = delegate;
    }

    void UnbindDelegate() {
        delegate_ = nullptr;
    }

private:
    Delegate * delegate_;
};


}  // namespace server
}  // user


#endif  // YGG_LIBUSER_SERVER_CONNECTOR_H_
