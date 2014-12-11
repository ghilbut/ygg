#include "object.h"
#include "object_ref.h"


namespace codebase {

ObjectRef& ObjectRef::operator= (ObjectRef& other) {
    if (other.object_ != NULL) {
        (other.object_)->AddRef();
    }
    if (object_ != NULL) {
        object_->Release();
    }
    object_ = other.object_;
    return *this;
}


ObjectRef::ObjectRef()
    : object_(NULL) {

}

ObjectRef::ObjectRef(Object* object)
    : object_(object) {
    // nothing
}

ObjectRef::ObjectRef(ObjectRef& other)
    : object_(other.object_) {
    if (object_ != NULL) {
        object_->AddRef();
    }
}

ObjectRef::~ObjectRef() {
    if (object_->Release() < 1) {
        object_ = NULL;
    }
}

} //  namespace codebase