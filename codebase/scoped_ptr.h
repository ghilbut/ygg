#ifndef CODEBASE_SCOPED_PTR_H_
#define CODEBASE_SCOPED_PTR_H_

#include <boost/noncopyable.hpp>


namespace codebase {


template<typename T>
class ScopedPtr : boost::noncopyable {
public:
	typedef void(*Deleter)(T*);

	explicit ScopedPtr(T* ptr, Deleter deleter = &Delete)
		: ptr_(ptr), deleter_(deleter) {
		// nothing
	}

	~ScopedPtr() {
		if (ptr_ != nullptr) {
			deleter_(ptr_);
		}
	}

	void Delete(T* ptr) {
		delete ptr;
	}

	T* Detach() {
		T* ret = ptr_;
		ptr_ = nullptr;
		return ret;
	}

	T* operator-> () const {
		return ptr_;
	}


private:
	T* ptr_;
	Deleter deleter_;
};


}  // codebase

#endif  // CODEBASE_SCOPED_PTR_H_