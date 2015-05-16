#ifndef YGG_CODEBASE_OBJECT_H_
#define YGG_CODEBASE_OBJECT_H_

#include <boost/intrusive_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <atomic>


namespace codebase {


template<class T>
void intrusive_ptr_add_ref(T * px) {
    px->add_ref();
}

template<class T>
void intrusive_ptr_release(T * px) {
    px->release();
}


template<class T>
class Object : public boost::noncopyable {
public:
    typedef boost::intrusive_ptr<T> Ptr;

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
            dispose();
        }
	}

    virtual void dispose() {
        delete this;
    }


protected:
	Object() : ref_count_({0}), weak_count_({1}) {}
	virtual ~Object() {}


private:
	std::atomic_int ref_count_;
    std::atomic_int weak_count_;
};


}  // codebase


#endif  // YGG_CODEBASE_OBJECT_H_
