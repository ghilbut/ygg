#ifndef YGG_CODEBASE_OBJECT_H_
#define YGG_CODEBASE_OBJECT_H_

#include <boost/intrusive_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <atomic>


namespace codebase {


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


template<class T> class CountHelper;
template<class T> class WeakPtr;


template<class T>
class Object : public boost::noncopyable {
public:
    typedef boost::intrusive_ptr<T> Ptr;
    typedef WeakPtr<T> Weak;

    // NOTE(ghilbut): hasher for unordered_set and unordered_map
    // usage - std::unordered_set<T::Ptr, T::Hash> s;
    //         std::unordered_map<T::Ptr, V, T::Hash> m;
    class Hash {
    public:
        size_t operator() (const typename T::Ptr & p) const {
            return std::hash<const T*>()(p.get());
        }
    };
    
    inline void AddRef() {
        counter_->AddRef();
    }

    inline void Release() {
        counter_->Release();
    }

protected:
    Object(T * t) : counter_(new Counter(t)) {}
    virtual ~Object() {}
    
private:
    typedef CountHelper<T> Counter;
    friend Counter;
    friend Weak;

    Counter * counter() const {
        return counter_;
    }

    virtual void Dispose() {
        delete this;
    }

private:
    Counter * counter_;
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
        return counter_->Lock();
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
    typename T::Counter * counter_;
};


template<class T>
class CountHelper : public boost::noncopyable {
public:
    CountHelper(T * px) : use_count_({0}), weak_counpx_({1}), px_(px) {}
    ~CountHelper() {}
      
    inline void AddRef(bool is_weak = false) {
        if (is_weak) {
            ++weak_counpx_;
        }
        else {
            ++use_count_;
        }
    }

    inline void Release(bool is_weak = false) {
        if (is_weak) {
            --weak_counpx_;
        }
        else {
            if (--use_count_ == 0) {
                px_->Dispose();
                --weak_counpx_;
            }
        }

        if (weak_counpx_ == 0) {
            delete this;
        }
    }

    inline bool Expired() const {
        return (px_ == nullptr || use_count_ == 0);
    }

    typename T::Ptr Lock() const {
        return (use_count_ == 0 ? nullptr : px_);
    }

private:
    std::atomic_int use_count_;
    std::atomic_int weak_counpx_;
    T * px_;
};


}  // codebase


#endif  // YGG_CODEBASE_OBJECT_H_
