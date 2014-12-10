#include "box.h"
#include "box_impl.h"
#include <boost/bind.hpp>



static bool is_big_endian(void) {
    static const int n = 1;
    return ((char *)&n)[0] == 0;
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
    }
    else {
        buffer[0] = tmp[3];
        buffer[1] = tmp[2];
        buffer[2] = tmp[1];
        buffer[3] = tmp[0];
    }
}



Box::Box(IOService& io_service, Tcp::resolver::iterator endpoint_iterator)
    : ObjectRef(new BoxImpl(io_service, endpoint_iterator)) {
    // nothing
}

Box::~Box() {
    // nothing
}

void Box::Write(const std::string& text) {
    BoxImpl* impl = Get<BoxImpl>();
    if (impl != NULL) {
        impl->Write(text);
    }
}

void Box::Close() {
    BoxImpl* impl = Get<BoxImpl>();
    if (impl != NULL) {
        impl->Close();
    }
}





BoxImpl::BoxImpl(IOService& io_service, Tcp::resolver::iterator endpoint_iterator)
    : Object()
    , io_service_(io_service)
    , socket_(io_service) {

    boost::asio::async_connect(
        socket_
        , endpoint_iterator
        , boost::bind(&BoxImpl::handle_connect, this, boost::asio::placeholders::error));
}

BoxImpl::~BoxImpl() {
    // nothing
}


void BoxImpl::Write(const std::string& text) {
    io_service_.post(boost::bind(&BoxImpl::do_write, this, text));
}

void BoxImpl::Close() {
    io_service_.post(boost::bind(&BoxImpl::do_close, this));
}


void BoxImpl::handle_connect(const boost::system::error_code& error) {
    if (!error) {
        do_read();
    }
}

void BoxImpl::handle_read_size(const boost::system::error_code& error) {

    if (!error) {
        read_size_ = DecodeSize(read_size_);
        read_buffer_.resize(read_size_, '\0');

        boost::asio::async_read(
            socket_
            , boost::asio::buffer(&read_buffer_[0], read_size_)
            , boost::bind(&BoxImpl::handle_read_data, this, boost::asio::placeholders::error));
    }
    else {
        do_close();
    }
}

void BoxImpl::handle_read_data(const boost::system::error_code& error) {

    if (!error) {
        printf("[BoxImpl::handle_read_data] %s\n", read_buffer_.c_str());
        do_read();
    }
    else {
        do_close();
    }
}

void BoxImpl::handle_write(const boost::system::error_code& error) {

    if (!error) {
        write_q_.pop_front();
        if (!write_q_.empty()) {

            std::string& text = write_q_.front();
            const uint32_t size = text.length();

            char buffer[4];
            EncodeSize(size, buffer);
            std::string data(buffer, buffer + 4);
            data += write_q_.front();

            boost::asio::async_write(socket_,
                boost::asio::buffer(&data[0], data.length()),
                boost::bind(&BoxImpl::handle_write, this, boost::asio::placeholders::error));
        }
    }
    else {
        do_close();
    }
}


void BoxImpl::do_read() {

    boost::asio::async_read(
        socket_
        , boost::asio::buffer(reinterpret_cast<char*>(&read_size_), sizeof(uint32_t))
        , boost::bind(&BoxImpl::handle_read_size, this, boost::asio::placeholders::error));
}

void BoxImpl::do_write(const std::string& text) {

    bool write_in_progress = !write_q_.empty();
    write_q_.push_back(text);
    if (!write_in_progress) {

        std::string& text = write_q_.front();
        const uint32_t size = text.length();

        char buffer[4];
        EncodeSize(size, buffer);
        std::string data(buffer, buffer + 4);
        data += write_q_.front();

        boost::asio::async_write(
            socket_,
            boost::asio::buffer(&data[0], data.length()),
            boost::bind(&BoxImpl::handle_write, this, boost::asio::placeholders::error));
    }
}

void BoxImpl::do_close() {
    socket_.close();
}