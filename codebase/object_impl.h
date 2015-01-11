#ifndef CODEBASE_OBJECT_IMPL_H_
#define CODEBASE_OBJECT_IMPL_H_

#include <boost/noncopyable.hpp>


namespace codebase {


class Object::Impl : public boost::noncopyable {
public:
	Impl() {}
	virtual ~Impl() {}
};


}  // codebase

#endif  // CODEBASE_OBJECT_IMPL_H_