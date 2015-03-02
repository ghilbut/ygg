#ifndef CODEBASE_Object2_H_
#define CODEBASE_Object2_H_

#include <boost/intrusive_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <atomic>


namespace codebase {


class Object2 : public boost::noncopyable {
public:
	typedef boost::intrusive_ptr<Object2> Ptr;

	inline void add_ref() {
		++ref_count_;
	}

	inline void release() {
		if (--ref_count_ < 1) {
			delete this;
		}
	}


protected:
	Object2() : ref_count_({ 0 }) {}
	virtual ~Object2() {}

private:
	std::atomic_int ref_count_;
};


}  // codebase

#endif  // CODEBASE_Object2_H_