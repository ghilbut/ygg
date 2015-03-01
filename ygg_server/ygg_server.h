#ifndef YGG_SERVER_H_
#define YGG_SERVER_H_

#include "codebase/io_service.h"
#include "codebase/net/http/http_server.h"


class YggServer {


public:
	YggServer();
	~YggServer();

	void Start();
	void Stop();

private:
	IOService io_service_;
	codebase::HttpServer httpd_;
};


#endif  // YGG_SERVER_H_