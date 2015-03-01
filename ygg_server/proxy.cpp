#include "proxy.h"
#include <boost/bind.hpp>


Proxy::Proxy(SessionPtr session) 
    : localbox_(session) {

    session->BindReadCallback(boost::bind(&Proxy::OnLocalBoxRead, this, _1, _2));
    session->BindErrorCallback(boost::bind(&Proxy::OnLocalBoxError, this, _1, _2));
}

Proxy::~Proxy() {

}

void Proxy::Join(SessionPtr session, const std::string& data) {
    clients_.insert(session);
}

void Proxy::OnLocalBoxRead(SessionPtr session, const std::string& data) {

    printf("[Proxy::OnLocalBoxRead] %s\n", data.c_str());
}

void Proxy::OnLocalBoxError(SessionPtr session, const boost::system::error_code& error) {
    localbox_.reset();

    printf("[Proxy::OnLocalBoxError] %s\n", error.message().c_str());
}


void Proxy::OnClientRead(SessionPtr session, const std::string& data) {

    printf("[Proxy::OnClientRead] %s\n", data.c_str());
}

void Proxy::OnClientError(SessionPtr session, const boost::system::error_code& error) {
    clients_.erase(session);

    printf("[Proxy::OnClientError] %s\n", error.message().c_str());
}
