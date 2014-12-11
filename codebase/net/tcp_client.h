#ifndef YGG_CODEBASE_NET_TCP_CLIENT_H_
#define YGG_CODEBASE_NET_TCP_CLIENT_H_

#include <boost/asio.hpp>
#include <deque>
#include <string>


namespace codebase {
namespace net {

class TcpClient {
public:
    explicit TcpClient(boost::asio::io_service& io_service);
    virtual ~TcpClient();

    void Connect(const std::string& host, int port);
    void Send(const std::string& text);
    void Close();

protected:
    virtual void OnConnect(const boost::system::error_code& error) = 0;
    virtual void OnError(const boost::system::error_code& error) = 0;
    virtual void OnRecv(const std::string& data) = 0;
    virtual void OnClosed() = 0;

private:
	class Impl;
	Impl* pimpl_;
};

}  // namespace net
}  // namespace codebase

#endif  // YGG_CODEBASE_NET_TCP_CLIENT_H_