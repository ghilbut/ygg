#ifndef CODEBASE_REFERENCE_PTR_H_
#define CODEBASE_REFERENCE_PTR_H_

#include <cassert>


namespace codebase {


template<class T, const bool is_weak = false>
class reference_ptr {
private:
    typedef reference_ptr this_type;
public:
    typedef T emelent_type;

    explicit reference_ptr(T *px = nullptr)
        : px_(px) {

        if (px_ != nullptr) {
            px_->add_ref(is_weak);
        }
    }

    explicit reference_ptr(reference_ptr &&rhs)
        : px_(rhs.px_) {
        rhs.px_ = nullptr;
    }

    explicit reference_ptr(reference_ptr const &rhs)
        : px_(rhs.get()) {

        if (px_ != nullptr) {
            px_->add_ref(is_weak);
        }
    }

    ~reference_ptr() {
        printf("DESTRUCTOR(%s) - %x\n", is_weak ? "W" : "R", px_);
        if (px_ != nullptr) {
            px_->release(is_weak);
        }
    }

    reference_ptr & operator=(reference_ptr const &rhs) {
        this_type(rhs).swap(*this);
        return *this;
    }

    reference_ptr & operator=(reference_ptr &&rhs) {
        this_type(static_cast<reference_ptr &&>(rhs)).swap(*this);
        return *this;
    }

    reference_ptr & operator=(T * rhs) {
        this_type(rhs).swap(*this);
        return *this;
    }

    void reset(T *rhs = nullptr) {
        this_type(rhs).swap(*this);
    }

    T * get() const {
        return px_;
    }

    T & operator*() const {
        assert(px_ != nullptr);
        return *px_;
    }

    T * operator->() const {
        assert(px_ != nullptr);
        return px_;
    }

    void swap(reference_ptr & rhs) {
        T *tmp = px_;
        px_ = rhs.px_;
        rhs.px_ = tmp;
    }

private:
    T *px_;
};


}  // namespace codebase

#endif  // CODEBASE_REFERENCE_PTR_H_
