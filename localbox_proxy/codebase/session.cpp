#include "session.h"
#include <boost/bind.hpp>


static bool is_big_endian(void) {
    static const int n = 1;
    return ((char *) &n)[0] == 0;
}

static uint32_t DecodeSize(uint32_t size) {

    if (is_big_endian()) {
        return size;
    }

    uint32_t ret;
    char* tmp = reinterpret_cast<char*>(&ret);
    const char* psize = reinterpret_cast<const char*>(&size);
    tmp[0] = psize[3];
    tmp[1] = psize[2];
    tmp[2] = psize[1];
    tmp[3] = psize[0];
    return ret;
}

static void EncodeSize(uint32_t size, char buffer[4]) {

    const char* tmp = reinterpret_cast<const char*>(&size);
    if (is_big_endian()) {
        buffer[0] = tmp[0];
        buffer[1] = tmp[1];
        buffer[2] = tmp[2];
        buffer[3] = tmp[3];
    } else {
        buffer[0] = tmp[3];
        buffer[1] = tmp[2];
        buffer[2] = tmp[1];
        buffer[3] = tmp[0];
    }
}


namespace codebase {

Session::Session(IOService& io_service)
    : socket_(io_service)
    , read_size_(0) {
    // nothing
}

Session::~Session() {
    // nothing
    printf("[Session::~Session]\n");
}

void Session::Start() {
    do_read();
}

void Session::BindReadCallback(ReadCallback callback) {
    onread_ = callback;
}

void Session::BindErrorCallback(ErrorCallback callback) {
    onerror_ = callback;
}

Tcp::socket& Session::socket() {
    return socket_;
}


void Session::handle_read_size(const boost::system::error_code& error) {

    if (error) {
        if (onerror_) {
            onerror_(shared_from_this(), error);
        }
    } else {
        read_size_ = DecodeSize(read_size_);
        read_data_.resize(read_size_, '\0');

        if (read_size_ < 1) {
            do_read();
        } else {
            boost::asio::async_read(
                socket_
                , boost::asio::buffer(&read_data_[0], read_size_)
                , boost::bind(&Session::handle_read_data, this, boost::asio::placeholders::error));
        }
    }
}

void Session::handle_read_data(const boost::system::error_code& error) {

    if (error) {
        if (onerror_) {
            onerror_(shared_from_this(), error);
        }
    } else {
        if (onread_) {
            onread_(shared_from_this(), read_data_);
        }
            
        do_read();
    }
}

void Session::handle_write(const boost::system::error_code& error) {

    if (error) {
        if (onerror_) {
            onerror_(shared_from_this(), error);
        }
    } else {
        if (!write_q_.empty()) {

            std::string& data = write_q_.front();

            boost::asio::async_write(
                socket_
                , boost::asio::buffer(&data[0], data.length())
                , boost::bind(&Session::handle_write, shared_from_this(), boost::asio::placeholders::error));
        }
    }
}

void Session::do_read() {
    boost::asio::async_read(
        socket_
        , boost::asio::buffer(reinterpret_cast<char*>(&read_size_), sizeof(read_size_))
        , boost::bind(&Session::handle_read_size, this, boost::asio::placeholders::error));
}

}  // namespace codebase