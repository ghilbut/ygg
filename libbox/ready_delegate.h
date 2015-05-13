#ifndef YGG_LIBBOX_READY_DELEGATE_H_
#define YGG_LIBBOX_READY_DELEGATE_H_

#include "ready.h"


namespace box {


class Proxy;


class Ready::Delegate {
public:
    virtual void OnReady(Proxy * proxy) = 0;

protected:
    virtual ~Delegate() {}
};


}  // namespace box


#endif  // YGG_LIBBOX_READY_DELEGATE_H_
