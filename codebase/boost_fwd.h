#ifndef YGG_CODEBASE_BOOST_FWD_H_
#define YGG_CODEBASE_BOOST_FWD_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>

typedef boost::asio::io_service IOService;
typedef boost::asio::ip::tcp    Tcp;
typedef boost::thread           Thread;

#endif  // YGG_CODEBASE_BOOST_FWD_H_