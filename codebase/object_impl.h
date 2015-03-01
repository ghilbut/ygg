#ifndef CODEBASE_OBJECT_IMPL_H_
#define CODEBASE_OBJECT_IMPL_H_

#include <boost/atomic.hpp>
#include <boost/noncopyable.hpp>


namespace codebase {


class Object::Impl : public boost::noncopyable {
public:
	explicit Impl(bool add_ref = true);
	virtual ~Impl() {}

	void add_ref();
	void release();

private:
	boost::atomic_int ref_count_;
};


}  // codebase

#endif  // CODEBASE_OBJECT_IMPL_H_