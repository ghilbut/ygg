#ifndef LOCALBOX_PROXY_LOCALBOX_DESC_IMPL_H_
#define LOCALBOX_PROXY_LOCALBOX_DESC_IMPL_H_

#include <boost/atomic.hpp>
#include <string>


class LocalBoxDesc::Impl {
private:
	Impl(const std::string& json);
	~Impl() {}
public:
	static Impl* New(const std::string& json);

	int AddRef() const;
	void Release() const;

	const char* id() const;


private:
	bool invalid_;
	mutable boost::atomic_int ref_count_;
	std::string id_;
};

#endif  // LOCALBOX_PROXY_LOCALBOX_DESC_IMPL_H_