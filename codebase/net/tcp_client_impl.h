#ifndef YGG_CODEBASE_NET_TCP_CLIENT_IMPL_H_
#define YGG_CODEBASE_NET_TCP_CLIENT_IMPL_H_

#include "boost_fwd.h"
#include <deque>
#include <string>


namespace codebase {
namespace net {

class TcpClient;

class TcpClient::Impl {
public:
	explicit Impl(IOService& io_service, TcpClient& delegate);
	virtual ~Impl();

    void Connect(const std::string& host, int port);
    void Send(const std::string& text);
    void Close();

protected:
    void OnConnect(const boost::system::error_code& error);
    void OnError(const boost::system::error_code& error);
    void OnRecv(const std::string& data);
    void OnClosed();

private:
    void handle_connect(const boost::system::error_code& error);
    void handle_read_size(const boost::system::error_code& error);
    void handle_read_data(const boost::system::error_code& error);
    void handle_write(const boost::system::error_code& error);

    void do_read();
    void do_write(const std::string& text);
    void do_write();
    void do_close();

private:
    IOService& io_service_;
    Tcp::socket socket_;

    uint32_t read_size_;
    std::string read_buffer_;
    std::deque<std::string> write_q_;

	TcpClient& delegate_;
};

}  // namespace net
}  // namespace codebase

#endif  // YGG_CODEBASE_NET_TCP_CLIENT_IMPL_H_