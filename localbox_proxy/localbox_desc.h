#ifndef LOCALBOX_PROXY_LOCALBOX_DESC_H_
#define LOCALBOX_PROXY_LOCALBOX_DESC_H_

#include <string>


class LocalBoxDesc {
public:
	LocalBoxDesc();
	explicit LocalBoxDesc(const std::string& json);
	explicit LocalBoxDesc(const LocalBoxDesc& other);
	~LocalBoxDesc();

	LocalBoxDesc& operator= (const LocalBoxDesc& other);
	bool operator== (const LocalBoxDesc& other);
	bool operator!= (const LocalBoxDesc& other);

	bool IsEmpty() const;

	const char* id() const;

private:
	class Impl;
	Impl* impl_;
};

#endif  // LOCALBOX_PROXY_LOCALBOX_DESC_H_