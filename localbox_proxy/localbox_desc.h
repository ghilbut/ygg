#ifndef LOCALBOX_PROXY_LOCALBOX_DESC_H_
#define LOCALBOX_PROXY_LOCALBOX_DESC_H_

#include <boost/shared_ptr.hpp>
#include <string>


class LocalBoxDesc {
public:
    LocalBoxDesc() {}
    ~LocalBoxDesc() {}

	explicit LocalBoxDesc(const std::string& json);
	explicit LocalBoxDesc(const LocalBoxDesc& other);

	LocalBoxDesc& operator= (const LocalBoxDesc& other);
	bool operator== (const LocalBoxDesc& other) const;
	bool operator!= (const LocalBoxDesc& other) const;

	bool IsNull() const;

	const char* id() const;

private:
	class Impl;
    boost::shared_ptr<Impl> impl_;
};

#endif  // LOCALBOX_PROXY_LOCALBOX_DESC_H_