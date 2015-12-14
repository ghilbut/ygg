#ifndef YGG_NET_DELEGATE_H_
#define YGG_NET_DELEGATE_H_

#include "define.h"


namespace ygg {
namespace net {


template<class T>
class Delegate {
public:
    virtual void OnText(T * t, const Text & text) = 0;
    virtual void OnBinary(T * t, const Bytes & bytes) = 0;
    virtual void OnClosed(T * t) = 0;

protected:
    virtual ~Delegate() {}
};

template<class T>
class NullDelegate : public Delegate<T> {
public:
    inline virtual void OnText(T*, const Text&) {}
    inline virtual void OnBinary(T*, const Bytes&) {}
    inline virtual void OnClosed(T*) {}
};


}  // namespace net
}  // namespace ygg


#endif  // YGG_NET_DELEGATE_H_
