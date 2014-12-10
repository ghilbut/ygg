#ifndef LOCALBOX_PROXY_MANAGER_H_
#define LOCALBOX_PROXY_MANAGER_H_

#include "codebase/server.h"
#include "codebase/session.h"
#include <boost/shared_ptr.hpp>
#include <map>
#include <set>

typedef boost::shared_ptr<codebase::Session> SessionPtr;


class Proxy;

class Manager {
    typedef boost::shared_ptr<Proxy> ProxyPtr;

public:
    explicit Manager(IOService& io_service);
    ~Manager();

    void OnAccept();

    // localbox
    void OnLocalBoxAccept(SessionPtr session, const boost::system::error_code& error);
    void OnLocalBoxRead(SessionPtr session, const std::string& data);
    void OnLocalBoxError(SessionPtr session, const boost::system::error_code& error);

    // client
    void OnClientAccept(SessionPtr session, const boost::system::error_code& error);
    void OnClientRead(SessionPtr session, const std::string& data);
    void OnClientError(SessionPtr session, const boost::system::error_code& error);


private:
    typedef std::map<std::string, ProxyPtr> ProxyPool;
    ProxyPool proxy_pool_;

    codebase::Server localbox_server_;
    codebase::Server client_server_;
    std::set<SessionPtr> ready_set_;
};

#endif  // LOCALBOX_PROXY_MANAGER_H_