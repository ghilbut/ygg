#include "object.h"


namespace ygg {


void intrusive_ptr_add_ref(Object * px) {
    if (px != nullptr) {
        px->AddRef();
    }
}

void intrusive_ptr_release(Object * px) {
    if (px != nullptr) {
        px->Release();
    }
}


void Object::AddRef() {
    counter_->AddRef();
}

void Object::Release() {
    counter_->Release();
}

Object::Object() 
    : counter_(new CountHelper(this)) {
    // nothing
}

Object::~Object() {
    // nothing
}

CountHelper * Object::counter() const {
    return counter_;
}

void Object::Dispose() {
    delete this;
}



CountHelper::CountHelper(Object * px) 
    : use_count_({0})
    , weak_count_({1})
    , px_(px) {
    // nothing
}

CountHelper::~CountHelper() {
    // nothing
}
      
void CountHelper::AddRef(bool is_weak) {
    if (is_weak) {
        ++weak_count_;
    }
    else {
        ++use_count_;
    }
}

void CountHelper::Release(bool is_weak) {
    if (is_weak) {
        --weak_count_;
    }
    else {
        if (--use_count_ == 0) {
            px_->Dispose();
            --weak_count_;
        }
    }

    if (weak_count_ == 0) {
        delete this;
    }
}

bool CountHelper::Expired() const {
    return (px_ == nullptr || use_count_ == 0);
}

Object * CountHelper::Lock() const {
    return (use_count_ == 0 ? nullptr : px_);
}


}  // namespace ygg
