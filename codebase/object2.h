#ifndef CODEBASE_OBJECT2_H_
#define CODEBASE_OBJECT2_H_

#include "shared_ptr.h"
#include <boost/noncopyable.hpp>
#include <atomic>


namespace codebase {


template<class T>
class Object2 : public boost::noncopyable {
public:
    typedef SharedPtr<T> Ptr;

	inline void add_ref(bool is_weak = false) {
        if (is_weak) {
            ++weak_count_;
        }
        else {
		    ++ref_count_;
        }
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

        if (weak_count_ == 0) {
            delete this;
        }
	}

    inline void MakeWeak() {
        ++weak_count_;
        --ref_count_;
    }

    inline void ClearWeak() {
        ++ref_count_;
        --weak_count_;
    }

protected:
	Object2() : ref_count_({0}), weak_count_({1}) {}
	virtual ~Object2() {}

private:
	std::atomic_int ref_count_;
    std::atomic_int weak_count_;
};


}  // codebase


#endif  // CODEBASE_OBJECT2_H_
