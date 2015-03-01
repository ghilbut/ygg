#ifndef YGG_SERVER_HTTP_SERVER_H_
#define YGG_SERVER_HTTP_SERVER_H_

extern "C" {
#include "mongoose.h"
}

class HttpServer {
public:
	void Start();
	void Stop();

private:
	mg_co
};

#endif  // YGG_SERVER_HTTP_SERVER_H_