#ifndef CODEBASE_OBJECT2_H_
#define CODEBASE_OBJECT2_H_

#include "reference_ptr.h"
#include <boost/noncopyable.hpp>
#include <atomic>


namespace codebase {


template<class T>
class Object2 : public boost::noncopyable {
public:
    typedef reference_ptr<T> Ptr;
    typedef reference_ptr<T, true> Weak;

	inline void add_ref(bool is_weak = false) {
        if (is_weak) {
            ++weak_count_;
        }
        else {
		    ++ref_count_;
        }

printf("[add_ref] %s %d:%d - %x\n", is_weak ? "W" : "R", ref_count_.load(), weak_count_.load(), (void*)this);
	}

	inline void release(bool is_weak = false) {

        if (is_weak) {
            --weak_count_;
        }
        else {
            if (--ref_count_ == 0) {
                --weak_count_;
            }
        }

printf("[release] %s %d:%d - %x\n", is_weak ? "W" : "R", ref_count_.load(), weak_count_.load(), (void*)this);

        if (weak_count_ == 0) {
            delete this;
        }
	}

protected:
	Object2() : ref_count_({0}), weak_count_({1}) {}
	virtual ~Object2() {}

private:
	std::atomic_int ref_count_;
    std::atomic_int weak_count_;
};


}  // codebase


template<typename T>
void intrusive_ptr_add_ref(T *pt) {
    pt->add_ref();
}

template<typename T>
void intrusive_ptr_release(T *pt) {
    pt->release();
}


#endif  // CODEBASE_OBJECT2_H_
