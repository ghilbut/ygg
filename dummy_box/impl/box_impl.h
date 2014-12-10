#ifndef YGG_DUMMY_BOX_BOX_IMPL_H_
#define YGG_DUMMY_BOX_BOX_IMPL_H_

#include "codebase/object.h"
#include <boost/asio.hpp>
#include <deque>
#include <string>

typedef boost::asio::io_service IOService;
typedef boost::asio::ip::tcp Tcp;


class BoxImpl : public codebase::Object {
public:
    BoxImpl(IOService& io_service, Tcp::resolver::iterator endpoint_iterator);
    ~BoxImpl();

    void Write(const std::string& text);
    void Close();

private:
    void handle_connect(const boost::system::error_code& error);
    void handle_read_size(const boost::system::error_code& error);
    void handle_read_data(const boost::system::error_code& error);
    void handle_write(const boost::system::error_code& error);

    void do_read();
    void do_write(const std::string& text);
    void do_close();

private:
    IOService& io_service_;
    Tcp::socket socket_;

    uint32_t read_size_;
    std::string read_buffer_;
    std::deque<std::string> write_q_;
};

#endif  // YGG_DUMMY_BOX_BOX_IMPL_H_