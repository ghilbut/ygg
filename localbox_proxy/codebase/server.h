#ifndef LOCALBOX_PROXY_CODEBASE_SERVER_H_
#define LOCALBOX_PROXY_CODEBASE_SERVER_H_

#include "session.h"
#include <set>


namespace codebase {

class Session;
typedef boost::shared_ptr<Session> SessionPtr;

class Server {

    typedef boost::function<void (SessionPtr, const boost::system::error_code&)> AcceptCallback;

public:
    Server(IOService& io_service, const Tcp::endpoint& endpoint);
    virtual ~Server();

    void BindAcceptCallback(AcceptCallback callback);
    void UnbindAcceptCallback();

private:
    void start_accept();
    void handle_accept(SessionPtr session, const boost::system::error_code& error);

private:
    IOService& io_service_;
    Tcp::acceptor acceptor_;

    AcceptCallback onaccept_;
};

}  // namespace codebase

#endif  // LOCALBOX_PROXY_CODEBASE_SERVER_H_