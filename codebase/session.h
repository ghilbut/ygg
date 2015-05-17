#ifndef YGG_CODEBASE_SESSION_H_
#define YGG_CODEBASE_SESSION_H_

#include "object.h"
#include <string>


namespace codebase {


class Session : public Object<Session> {
public:
    class Delegate;

    virtual size_t SendText(const std::string & text) const = 0;
    virtual size_t SendBinary(const uint8_t bytes[], size_t size) const = 0;
    virtual void Close() = 0;

    void FireOnTextEvent(const std::string & text);
    void FireOnBinaryEvent(const uint8_t bytes[], size_t size);
    void FireOnClosedEvent();

    void BindDelegate(Delegate * delegate);
    void UnbindDelegate();

protected:
    Session();
    virtual ~Session() {}

private:
    Delegate * delegate_;
};


}  // namespace codebase


#endif  // YGG_CODEBASE_SESSION_H_
