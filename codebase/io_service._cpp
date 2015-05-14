#include "io_service.h"
#include <boost/bind.hpp>


IOService::IOService() {
	// nothing
}

IOService::~IOService() {
	// nothing
}

void IOService::Start() {

	if (!io_service_.stopped()) {
		return;
	}

	auto runnable = boost::bind(&boost::asio::io_service::run, &io_service_);
	thread_.swap(boost::thread(runnable));
}

void IOService::Stop() {

	if (!io_service_.stopped()) {
		io_service_.stop();
		io_service_.reset();
		thread_.join();
	}
}