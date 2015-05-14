#ifndef YGG_CODEBASE_READY_DELEGATE_H_
#define YGG_CODEBASE_READY_DELEGATE_H_

#include "ready.h"


namespace codebase {


class Proxy;


class Ready::Delegate {
public:
    virtual void OnReady(Proxy * proxy) = 0;

protected:
    virtual ~Delegate() {}
};


}  // namespace codebase


#endif  // YGG_CODEBASE_READY_DELEGATE_H_
