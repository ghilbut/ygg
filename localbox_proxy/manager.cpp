#include "manager.h"
#include "proxy.h"
#include <json/json.h>
#include <boost/bind.hpp>


Manager::Manager(IOService& io_service)
    : localbox_server_(io_service, Tcp::endpoint(Tcp::v4(), 8080)) 
    , client_server_(io_service, Tcp::endpoint(Tcp::v4(), 8090)) {

    localbox_server_.BindAcceptCallback(boost::bind(&Manager::OnLocalBoxAccept, this, _1, _2));
    client_server_.BindAcceptCallback(boost::bind(&Manager::OnClientAccept, this, _1, _2));
}

Manager::~Manager() {
    client_server_.UnbindAcceptCallback();
    localbox_server_.UnbindAcceptCallback();
}


void Manager::OnLocalBoxAccept(SessionPtr session, const boost::system::error_code& error) {

    printf("[Manager::OnLocalBoxAccept]");

    if (error) {
        // TODO(ghilbut): error handling
        printf(" %s", error.message().c_str());
    } else {
        session->BindReadCallback(boost::bind(&Manager::OnLocalBoxRead, this, _1, _2));
        session->BindErrorCallback(boost::bind(&Manager::OnLocalBoxError, this, _1, _2));
        ready_set_.insert(session);
        session->Start();
    }

    printf("\n");
}

void Manager::OnLocalBoxRead(SessionPtr session, const std::string& data) {

    Json::Value root;
    Json::Reader reader;
    if (reader.parse(data, root, false)) {
        
        Json::Value id = root.get("id", Json::Value(Json::nullValue));
        if (id.isNull()) {
            // TODO(ghilbut): error handling
        }

        ProxyPtr proxy(new Proxy(session));
        proxy_pool_[id.asString()] = proxy;
        ready_set_.erase(session);

        printf("[Manager::OnLocalBoxRead] %s\n", data.c_str());

    } else {
        // TODO(ghilbut): error handling
        ready_set_.erase(session);
    }
}

void Manager::OnLocalBoxError(SessionPtr session, const boost::system::error_code& error) {
    ready_set_.erase(session);

    printf("[Manager::OnLocalBoxError] %s\n", error.message().c_str());
}


void Manager::OnClientAccept(SessionPtr session, const boost::system::error_code& error) {

    printf("[Manager::OnClientAccept]");

    if (error) {
        // TODO(ghilbut): error handling
        printf(" %s", error.message().c_str());
    } else {
        session->BindReadCallback(boost::bind(&Manager::OnClientRead, this, _1, _2));
        session->BindErrorCallback(boost::bind(&Manager::OnClientError, this, _1, _2));
        ready_set_.insert(session);
        session->Start();
    }

    printf("\n");
}

void Manager::OnClientRead(SessionPtr session, const std::string& data) {

    Json::Value root;
    Json::Reader reader;
    if (reader.parse(data, root, false)) {

        Json::Value id = root.get("id", Json::Value(Json::nullValue));
        if (id.isNull()) {
            // TODO(ghilbut): error handling
        }

        ProxyPool::iterator itr = proxy_pool_.find(id.asString());
        if (itr == proxy_pool_.end()) {
            // TODO(ghilbut): error handling
        }

        ProxyPtr& proxy = itr->second;
        proxy->Join(session, data);
        ready_set_.erase(session);

        printf("[Manager::OnClientRead] %s\n", data.c_str());

    } else {
        // TODO(ghilbut): error handling
        ready_set_.erase(session);
    }
}

void Manager::OnClientError(SessionPtr session, const boost::system::error_code& error) {
    ready_set_.erase(session);

    printf("[Manager::OnClientError] %s\n", error.message().c_str());
}
