#ifndef LOCALBOX_PROXY_LOCALBOX_H_
#define LOCALBOX_PROXY_LOCALBOX_H_

#include "http_fwd.h"
#include "codebase/object.h"
#include <string>


class LocalBox : public codebase::Object {
public:
	LocalBox() {}
	~LocalBox() {}

	LocalBox(const std::string &json, HttpWebsocket &ws);

private:
	class Impl;
};

#endif  // LOCALBOX_PROXY_LOCALBOX_H_