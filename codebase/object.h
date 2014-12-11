#ifndef YGG_DUMMY_BOX_OBJECT_H_
#define YGG_DUMMY_BOX_OBJECT_H_

#include <atomic>


namespace codebase {

class Object
{
public:
    int AddRef();
    int Release();

protected:
    Object();
    virtual ~Object();

private:
    std::atomic_int ref_count_;
    // std::atomic_int weak_count_;
};

}  // namespace codebase

#endif  // YGG_DUMMY_BOX_OBJECT_H_