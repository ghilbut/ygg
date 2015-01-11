#ifndef LOCALBOX_PROXY_IMPL_LOCALBOX_DESC_IMPL_H_
#define LOCALBOX_PROXY_IMPL_LOCALBOX_DESC_IMPL_H_

#include "codebase/object_impl.h"
#include <string>


class LocalBoxDesc::Impl : public codebase::Object::Impl {
	Impl() {}
	~Impl() {}

public:
	static Impl* New(const std::string& json);
	static void Delete(Impl* impl);

	const char* id() const;


private:
	std::string id_;
};

#endif  // LOCALBOX_PROXY_IMPL_LOCALBOX_DESC_IMPL_H_