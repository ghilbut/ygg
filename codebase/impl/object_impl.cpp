#include "object.h"
#include "object_impl.h"


namespace codebase {


Object& Object::operator= (const Object& other) {
	impl_ = other.impl_;
	return *this;
}

bool Object::operator== (const Object& other) const {
	return (impl_ == other.impl_);
}

bool Object::operator!= (const Object& other) const {
	return (impl_ != other.impl_);
}

bool Object::operator== (std::nullptr_t) const {
	return (impl_.get() == 0);
}

bool Object::operator!= (std::nullptr_t) const {
	return (impl_.get() != 0);
}

bool Object::operator< (const Object& other) const {
	return (impl_.get() < other.impl_.get());
}


Object::Object(Impl* impl)
	: impl_(impl) {
	// nothing
}

Object::Object(const Object& other)
	: impl_(other.impl_) {
	// nothing
}


}  // codebase