#ifndef YGG_CODEBASE_OBJECT_H_
#define YGG_CODEBASE_OBJECT_H_

#include <boost/intrusive_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <atomic>


// NOTE(ghilbut): hasher for unordered_set and unordered_map
template<class T>
class std::hash<boost::intrusive_ptr<T>> {
public:
    size_t operator() (const typename T::Ptr & p) const {
        return std::hash<const T*>()(p.get());
    }
};
  

namespace ygg {

/*
template<class T>
void intrusive_ptr_add_ref(T * px) {
    if (px != nullptr) {
        px->AddRef();
    }
}

template<class T>
void intrusive_ptr_release(T * px) {
    if (px != nullptr) {
        px->Release();
    }
}
*/





class CountHelper;
template<class T> class WeakPtr;


class Object : public boost::noncopyable {
public:
    void AddRef();
    void Release();

    CountHelper * counter() const;

protected:
    Object();
    virtual ~Object();
    
private:
    friend CountHelper;
    //friend Weak;
    
    virtual void Dispose();

private:
    CountHelper * counter_;
};


void intrusive_ptr_add_ref(Object * px);
void intrusive_ptr_release(Object * px);


class CountHelper : public boost::noncopyable {
public:
    CountHelper(Object * px);
    ~CountHelper();
      
    void AddRef(bool is_weak = false);
    void Release(bool is_weak = false);

    bool Expired() const;

    Object * Lock() const;

private:
    std::atomic_int use_count_;
    std::atomic_int weak_count_;
    Object * px_;
};


template<class T>
class WeakPtr {
private:
    static const bool kIsWeak = true;

public:
    WeakPtr() : counter_(nullptr) {
        // nothing
    }

    WeakPtr(T * px) : counter_(nullptr) {
        if (px != nullptr) {
            counter_ = px->counter();
            counter_->AddRef(kIsWeak);
        }
    }

    WeakPtr(const WeakPtr & other) : counter_(other.counter()) {
        if (counter_ != nullptr) {
            counter_->AddRef(kIsWeak);
        }
    }

    WeakPtr(const typename T::Ptr & other)
        : counter_(nullptr) {

        if (other != nullptr) {
            counter_ = other->counter();
            counter_->AddRef(kIsWeak);
        }
    }

    ~WeakPtr() {
        if (counter_ != nullptr) {
            counter_->Release(kIsWeak);
        }
    }

    bool Expired() const {
        return (counter_ == nullptr || counter_->Expired());
    }

    typename T::Ptr Lock() const {
        if (counter_ == nullptr) {
            return nullptr;
        }
        return static_cast<T*>(counter_->Lock());
    }

    WeakPtr & operator = (T * px) {
        if (px != nullptr) {
            px->counter()->AddRef(kIsWeak);
        }
        if (counter_ != nullptr) {
            counter_->Release(kIsWeak);
        }
        counter_ = px->counter();
        return *this;
    }

    WeakPtr & operator = (const WeakPtr & other) {
        return (*this = other.px_);
    }

    WeakPtr & operator = (const typename T::Ptr & other) {
        return (*this = other.get());
    }

private:
    T * px_;
    CountHelper * counter_;
};


}  // ygg


#endif  // YGG_CODEBASE_OBJECT_H_
