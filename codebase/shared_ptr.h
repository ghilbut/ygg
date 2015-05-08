#ifndef YGG_CODEBASE_SHARED_PTR_H_
#define YGG_CODEBASE_SHARED_PTR_H_

// NOTE(ghilbut): this code came from boost::intrusive_ptr template.

#include <atomic>
#include <functional>
#include <cassert>


template<class T>
class SharedPtr {
private:

    typedef SharedPtr ThisType;

public:

    typedef T ElementType;

    SharedPtr() 
        : is_weak_({false}), px_(nullptr) {
    }

    SharedPtr(T * px, bool add_ref = true)
        : is_weak_({false}), px_(px) {
        if (px_ != nullptr && add_ref) {
            px_->add_ref(is_weak_);
        }
    }

    SharedPtr(SharedPtr const & rhs)
        : is_weak_({rhs.is_weak_.load()}), px_(rhs.px_) {
        if (px_ != nullptr) {
            px_->add_ref(is_weak_);
        }
    }

    ~SharedPtr() {
        if (px_ != nullptr) {
            px_->release(is_weak_);
        }
    }

    SharedPtr(SharedPtr && rhs) 
        : px_(rhs.px_) {
        rhs.px_ = nullptr;
    }

    SharedPtr & operator=(SharedPtr && rhs) {
        ThisType(static_cast<SharedPtr &&>(rhs)).Swap(*this);
        return *this;
    }

    SharedPtr & operator=(SharedPtr const & rhs) {
        ThisType(rhs).Swap(*this);
        return *this;
    }

    SharedPtr & operator=(T * rhs) {
        ThisType(rhs).Swap(*this);
        return *this;
    }

    void Reset() {
        ThisType().Swap(*this);
    }

    void Reset(T * rhs) {
        ThisType(rhs).Swap(*this);
    }

    void Reset(T * rhs, bool add_ref) {
        ThisType(rhs, add_ref).Swap(*this);
    }

    T * Get() const {
        return px_;
    }

    T * detach() {
        T * ret = px_;
        px_ = nullptr;
        return ret;
    }

    T & operator*() const {
        assert(px_ != nullptr);
        return *px_;
    }

    T * operator->() const {
        assert(px_ != nullptr);
        return px_;
    }

    void Swap(SharedPtr & rhs) {
        T * tmp = px_;
        px_ = rhs.px_;
        rhs.px_ = tmp;

        //bool is_weak = is_weak_.load();
        //is_weak_ = rhs.is_weak_.load();
        //rhs.is_weak_ = is_weak;
        if (is_weak_ && !rhs.is_weak_) {
            if (px_ != nullptr) {
                px_->MakeWeak();
            }
            rhs.ClearWeak();
        }

        if (!is_weak_ && rhs.is_weak_) {
            if (px_ != nullptr) {
                px_->ClearWeak();
            }
            rhs.MakeWeak();
        }
    }

    void MakeWeak() {
        if (!is_weak_) {
            is_weak_ = true;
            if (px_ != nullptr) {
                px_->MakeWeak();
            }
        }
    }

    void ClearWeak() {
        if (is_weak_) {
            is_weak_ = false;
            if (px_ != nullptr) {
                px_->ClearWeak();
            }
        }
    }

    bool IsWeak() {
        return is_weak_;
    }

private:
    std::atomic_bool is_weak_;
    T * px_;
};

template<class T, class U>
inline bool operator==(SharedPtr<T> const & a, SharedPtr<U> const & b) {
    return a.Get() == b.Get();
}

template<class T, class U>
inline bool operator!=(SharedPtr<T> const & a, SharedPtr<U> const & b) {
    return a.Get() != b.Get();
}

template<class T, class U>
inline bool operator==(SharedPtr<T> const & a, U * b) {
    return a.Get() == b;
}

template<class T, class U>
inline bool operator!=(SharedPtr<T> const & a, U * b) {
    return a.Get() != b;
}

template<class T, class U>
inline bool operator==(T * a, SharedPtr<U> const & b) {
    return a == b.Get();
}

template<class T, class U>
inline bool operator!=(T * a, SharedPtr<U> const & b) {
    return a != b.Get();
}

template<class T>
inline bool operator==(SharedPtr<T> const & p, std::nullptr_t) {
    return p.Get() == nullptr;
}

template<class T>
inline bool operator==(std::nullptr_t, SharedPtr<T> const & p) {
    return p.Get() == nullptr;
}

template<class T>
inline bool operator!=(SharedPtr<T> const & p, std::nullptr_t) {
    return p.Get() != nullptr;
}

template<class T>
inline bool operator!=(std::nullptr_t, SharedPtr<T> const & p) {
    return p.Get() != nullptr;
}

template<class T>
inline bool operator<(SharedPtr<T> const & a, SharedPtr<T> const & b) {
    return std::less<T *>()(a.Get(), b.Get());
}

template<class T>
void Swap(SharedPtr<T> & lhs, SharedPtr<T> & rhs) {
    lhs.Swap(rhs);
}

template<class T>
T * get_pointer(SharedPtr<T> const & p) {
    return p.Get();
}

template<class T, class U>
SharedPtr<T> static_pointer_cast(SharedPtr<U> const & p) {
    return static_cast<T *>(p.Get());
}

template<class T, class U>
SharedPtr<T> const_pointer_cast(SharedPtr<U> const & p) {
    return const_cast<T *>(p.Get());
}

template<class T, class U>
SharedPtr<T> dynamic_pointer_cast(SharedPtr<U> const & p) {
    return dynamic_cast<T *>(p.Get());
}


#endif  // #ifndef YGG_CODEBASE_SHARED_PTR_H_
