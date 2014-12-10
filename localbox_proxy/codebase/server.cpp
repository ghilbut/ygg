#include "server.h"
#include <boost/bind.hpp>


namespace codebase {

Server::Server(IOService& io_service, const Tcp::endpoint& endpoint)
    : io_service_(io_service)
    , acceptor_(io_service, endpoint) {
    start_accept();
}

Server::~Server() {

}

void Server::BindAcceptCallback(AcceptCallback callback) {
    onaccept_ = callback;
}

void Server::UnbindAcceptCallback() {
    onaccept_.clear();
}

void Server::start_accept() {
    SessionPtr session(new Session(io_service_));

    acceptor_.async_accept(
        session->socket()
        , boost::bind(&Server::handle_accept, this, session, boost::asio::placeholders::error));
}

void Server::handle_accept(SessionPtr session, const boost::system::error_code& error) {
    onaccept_(session, error);
    start_accept();
}

}  // namespace codebase