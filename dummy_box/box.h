#ifndef YGG_DUMMY_BOX_BOX_H_
#define YGG_DUMMY_BOX_BOX_H_

#include "codebase/object_ref.h"
#include <boost/asio.hpp>

typedef boost::asio::io_service IOService;
typedef boost::asio::ip::tcp Tcp;


class Box : public codebase::ObjectRef {
public:
    Box(IOService& io_service, Tcp::resolver::iterator endpoint_iterator);
    ~Box();

    void Write(const std::string& text);
    void Close();
};

#endif  // YGG_DUMMY_BOX_BOX_H_