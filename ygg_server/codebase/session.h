#ifndef LOCALBOX_PROXY_CODEBASE_SESSION_H_
#define LOCALBOX_PROXY_CODEBASE_SESSION_H_

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <deque>
#include <cstdint>

typedef boost::asio::io_service IOService;
typedef boost::asio::ip::tcp Tcp;


namespace codebase {

class Session : public boost::enable_shared_from_this<Session> {

    typedef std::deque<std::string> WriteQueue;

    typedef boost::shared_ptr<Session> SessionPtr;
    typedef boost::function<void (SessionPtr, const std::string&)> ReadCallback;
    typedef boost::function<void (SessionPtr, const boost::system::error_code&)> ErrorCallback;

public:
    explicit Session(IOService& io_service);
    ~Session();

    void Start();
    void BindReadCallback(ReadCallback callback);
    void BindErrorCallback(ErrorCallback callback);

    Tcp::socket& socket();

private:
    void handle_read_size(const boost::system::error_code& error);
    void handle_read_data(const boost::system::error_code& error);
    void handle_write(const boost::system::error_code& error);
    void do_read();

private:
    ReadCallback onread_;
    ErrorCallback onerror_;
    Tcp::socket socket_;

    uint32_t    read_size_;
    std::string read_data_;
    WriteQueue  write_q_;
};

}  // namespace codebase

#endif  // LOCALBOX_PROXY_CODEBASE_SESSION_H_