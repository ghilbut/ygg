#include "tcp_client.h"
#include "tcp_client_impl.h"
#include "util.h"
#include <boost/bind.hpp>


namespace codebase {
namespace net {


TcpClient::TcpClient(IOService& io_service)
	: pimpl_(new Impl(io_service, *this)) {
	// nothing
}

TcpClient::~TcpClient() {
	delete pimpl_;
}


void TcpClient::Connect(const std::string& host, int port) {
	pimpl_->Connect(host, port);
}

void TcpClient::Send(const std::string& text) {
	pimpl_->Send(text);
}

void TcpClient::Close() {
	pimpl_->Close();
}


TcpClient::Impl::Impl(IOService& io_service, TcpClient& delegate)
    : io_service_(io_service)
    , socket_(io_service)
	, delegate_(delegate) {
    // nothing
}

TcpClient::Impl::~Impl() {
    // nothing
}


void TcpClient::Impl::Connect(const std::string& host, int port) {

    char sport[10];
    sprintf(sport, "%d", port);

    Tcp::resolver resolver(io_service_);
    Tcp::resolver::query query(host, sport);
    Tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    boost::asio::async_connect(
        socket_
        , endpoint_iterator
		, boost::bind(&TcpClient::Impl::handle_connect, this, boost::asio::placeholders::error));
}

void TcpClient::Impl::Send(const std::string& text) {
	io_service_.post(boost::bind(&TcpClient::Impl::do_write, this, text));
}

void TcpClient::Impl::Close() {
	io_service_.post(boost::bind(&TcpClient::Impl::do_close, this));
}


void TcpClient::Impl::OnConnect(const boost::system::error_code& error) {
	delegate_.OnConnect(error);
}

void TcpClient::Impl::OnError(const boost::system::error_code& error) {
	delegate_.OnError(error);
}

void TcpClient::Impl::OnRecv(const std::string& data) {
	delegate_.OnRecv(data);
}

void TcpClient::Impl::OnClosed() {
	delegate_.OnClosed();
}


void TcpClient::Impl::handle_connect(const boost::system::error_code& error) {
    OnConnect(error);
    if (!error) {
        do_read();
    }
}

void TcpClient::Impl::handle_read_size(const boost::system::error_code& error) {

    if (!error) {
        read_size_ = codebase::net::DecodeSize(read_size_);
        read_buffer_.resize(read_size_, '\0');

        boost::asio::async_read(
            socket_
            , boost::asio::buffer(&read_buffer_[0], read_size_)
			, boost::bind(&TcpClient::Impl::handle_read_data, this, boost::asio::placeholders::error));
    } else {
        OnError(error);
        do_close();
    }
}

void TcpClient::Impl::handle_read_data(const boost::system::error_code& error) {

    if (!error) {
        OnRecv(read_buffer_);
        do_read();
    } else {
        OnError(error);
        do_close();
    }
}

void TcpClient::Impl::handle_write(const boost::system::error_code& error) {

    if (!error) {
        write_q_.pop_front();
        if (!write_q_.empty()) {
            do_write();
        }
    } else {
        OnError(error);
        do_close();
    }
}


void TcpClient::Impl::do_read() {

    boost::asio::async_read(
        socket_
        , boost::asio::buffer(reinterpret_cast<char*>(&read_size_), sizeof(uint32_t))
		, boost::bind(&TcpClient::Impl::handle_read_size, this, boost::asio::placeholders::error));
}

void TcpClient::Impl::do_write(const std::string& text) {

    bool write_in_progress = !write_q_.empty();
    write_q_.push_back(text);
    if (!write_in_progress) {
        do_write();
    }
}

void TcpClient::Impl::do_write() {

    std::string& text = write_q_.front();
    const uint32_t size = text.length();

    char buffer[4];
    codebase::net::EncodeSize(size, buffer);
    std::string data(buffer, buffer + 4);
    data += write_q_.front();

    boost::asio::async_write(
        socket_,
        boost::asio::buffer(&data[0], data.length()),
		boost::bind(&TcpClient::Impl::handle_write, this, boost::asio::placeholders::error));
}

void TcpClient::Impl::do_close() {
    socket_.close();
    OnClosed();
}


}  // namespace net
}  // namespace codebase