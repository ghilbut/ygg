#ifndef CODEBASE_IO_SERVICE_H_
#define CODEBASE_IO_SERVICE_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>


class IOService {
public:
	IOService();
	~IOService();

	void Start();
	void Stop();

	template <typename CompletionHandler>
	inline BOOST_ASIO_INITFN_RESULT_TYPE(CompletionHandler, void())
	Send(BOOST_ASIO_MOVE_ARG(CompletionHandler) handler) {
		return io_service_.dispatch(handler);
	}

	template <typename CompletionHandler>
	inline BOOST_ASIO_INITFN_RESULT_TYPE(CompletionHandler, void())
	Post(BOOST_ASIO_MOVE_ARG(CompletionHandler) handler) {
		return io_service_.post(handler);
	}

private:
	boost::asio::io_service io_service_;
	boost::thread thread_;
};


#endif  // CODEBASE_IO_SERVICE_H_