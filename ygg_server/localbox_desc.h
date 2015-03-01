#ifndef LOCALBOX_PROXY_LOCALBOX_DESC_H_
#define LOCALBOX_PROXY_LOCALBOX_DESC_H_

#include "codebase/object.h"
#include <string>


class LocalBoxDesc : public codebase::Object {
public:
    LocalBoxDesc() {}
    ~LocalBoxDesc() {}

	explicit LocalBoxDesc(const std::string& json);

	const char* id() const;

private:
	class Impl;
};

#endif  // LOCALBOX_PROXY_LOCALBOX_DESC_H_