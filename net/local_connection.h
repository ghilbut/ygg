#ifndef YGG_NET_LOCAL_CONNECTION_H_
#define YGG_NET_LOCAL_CONNECTION_H_

#include "connection.h"


namespace ygg {
namespace net {


class LocalConnection : public Connection {
public:
    static Connection::Ptr New();

    virtual size_t SendText(const Text & text) const;
    virtual size_t SendBinary(const Bytes & bytes) const;
    virtual void Close();

    Connection::Ptr other() const;

private:
    LocalConnection();
    LocalConnection(Connection * conn);
    ~LocalConnection();

private:
    LocalConnection * other_;
    //Connection::Weak conn_;
    WeakPtr<Connection> conn_;
};


}  // namespace net
}  // namespace ygg


#endif  // YGG_NET_LOCAL_CONNECTION_H_
