#ifndef LOCALBOX_PROXY_IMPL_LOCALBOX_DESC_IMPL_H_
#define LOCALBOX_PROXY_IMPL_LOCALBOX_DESC_IMPL_H_

#include <boost/noncopyable.hpp>
#include <string>


class LocalBoxDesc::Impl : public boost::noncopyable {
private:
    Impl() {}
    ~Impl() {}

public:
    typedef boost::shared_ptr<Impl> Ptr;
	static Ptr New(const std::string& json);
    static void Delete(Impl* impl);

	const char* id() const;


private:
	std::string id_;
};

#endif  // LOCALBOX_PROXY_IMPL_LOCALBOX_DESC_IMPL_H_