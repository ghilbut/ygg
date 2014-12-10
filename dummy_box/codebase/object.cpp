#include "object.h"


namespace codebase {

int Object::AddRef() {
    return ref_count_++;
}

int Object::Release() {
    if (--ref_count_ < 1) {
        delete this;
    }
    return ref_count_;
}



Object::Object() {
    ref_count_ = 1;
}


Object::~Object() {
    // nothing
}

}  // namespace codebase