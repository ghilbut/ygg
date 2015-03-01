#ifndef LOCALBOX_PROXY_PROXY_H_
#define LOCALBOX_PROXY_PROXY_H_

#include "codebase/session.h"
#include <boost/shared_ptr.hpp>
#include <set>

typedef boost::shared_ptr<codebase::Session> SessionPtr;


class Proxy {
public:
    explicit Proxy(SessionPtr session);
    ~Proxy();

    void Join(SessionPtr session, const std::string& data);

    // localbox
    void OnLocalBoxRead(SessionPtr session, const std::string& data);
    void OnLocalBoxError(SessionPtr session, const boost::system::error_code& error);

    // client
    void OnClientRead(SessionPtr session, const std::string& data);
    void OnClientError(SessionPtr session, const boost::system::error_code& error);

private:
    SessionPtr localbox_;
    std::set<SessionPtr> clients_;
};

#endif  // LOCALBOX_PROXY_PROXY_H_