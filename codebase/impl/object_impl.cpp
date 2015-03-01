#include "object.h"
#include "object_impl.h"


namespace codebase {


Object& Object::operator= (const Object& other) {
	if (impl_ != nullptr) {
		impl_->release();
	}
	impl_ = other.impl_;
	if (impl_ != nullptr) {
		impl_->add_ref();
	}
	return *this;
}

bool Object::operator== (const Object& other) const {
	return (impl_ == other.impl_);
}

bool Object::operator!= (const Object& other) const {
	return (impl_ != other.impl_);
}

bool Object::operator== (std::nullptr_t) const {
	return (impl_ == nullptr);
}

bool Object::operator!= (std::nullptr_t) const {
	return (impl_ != nullptr);
}

bool Object::operator< (const Object& other) const {
	return (impl_ < other.impl_);
}


Object::Object(Impl* impl)
	: impl_(impl) {

	if (impl_ != nullptr) {
		impl_->add_ref();
	}
}

Object::Object(const Object& other)
	: impl_(other.impl_) {
	
	if (impl_ != nullptr) {
		impl_->add_ref();
	}
}

Object::~Object() {
	if (impl_ != nullptr) {
		impl_->release();
	}
}


Object::Impl::Impl(bool add_ref)
	: ref_count_(add_ref ? 1 : 0) {
	// nothing
}

void Object::Impl::add_ref() {
	++ref_count_;
}

void Object::Impl::release() {
	--ref_count_;
	if (ref_count_ < 1) {
		delete this;
	}
}


}  // codebase