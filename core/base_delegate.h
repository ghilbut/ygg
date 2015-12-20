#ifndef YGG_CORE_BASE_DELEGATE_H_
#define YGG_CORE_BASE_DELEGATE_H_

#include "define.h"


namespace ygg {


template<class T>
class BaseDelegate {
public:
    virtual void OnText(T * t, const Text & text) = 0;
    virtual void OnBinary(T * t, const Bytes & bytes) = 0;
    virtual void OnClosed(T * t) = 0;

protected:
    virtual ~BaseDelegate() {}
};


}  // namespace ygg


#endif  // YGG_CORE_BASE_DELEGATE_H_
