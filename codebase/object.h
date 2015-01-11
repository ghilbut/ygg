#ifndef CODEBASE_OBJECT_H_
#define CODEBASE_OBJECT_H_

#include <boost/shared_ptr.hpp>


namespace codebase {


class Object {
public:
	Object& operator= (const Object& other);
	bool operator== (const Object& other) const;
	bool operator!= (const Object& other) const;

	bool operator== (std::nullptr_t) const;
	bool operator!= (std::nullptr_t) const;

	bool operator< (const Object& other) const;


protected:
	class Impl;
	boost::shared_ptr<Impl> impl_;

	Object() {}
	explicit Object(Impl* impl);
	explicit Object(const Object& other);
	virtual ~Object() {}
};


}  // codebase

#endif  // CODEBASE_OBJECT_H_