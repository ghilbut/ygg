#ifndef YGG_LIBBOX_USER_READY_H_
#define YGG_LIBBOX_USER_READY_H_

#include "codebase/ready.h"
#include "codebase/session_delegate.h"
#include <set>


using namespace codebase;


namespace box {


class UserReady : public Ready {
public:
    UserReady() {}
    ~UserReady() {}

    virtual void OnText(Session * session, const std::string & text);
    virtual void OnBinary(Session * session, const uint8_t bytes[], size_t size);


private:
};


}  // namespace box


#endif  // YGG_LIBBOX_USER_READY_H_
