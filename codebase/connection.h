#ifndef YGG_CODEBASE_CONNECTION_H_
#define YGG_CODEBASE_CONNECTION_H_

#include "object.h"
#include <string>
#include <vector>
#include <cstdint>


namespace codebase {


class Connection : public Object<Connection> {
public:
    class Delegate {
    public:
        virtual void OnText(Connection * conn, const std::string & text) = 0;
        virtual void OnBinary(Connection * conn, const std::vector<uint8_t> & bytes) = 0;
        virtual void OnClosed(Connection * conn) = 0;
    };

public:
    virtual size_t SendText(const std::string & text) const = 0;
    virtual size_t SendBinary(const std::vector<uint8_t> & bytes) const = 0;
    virtual void Close() = 0;

    void FireOnTextEvent(const std::string & text);
    void FireOnBinaryEvent(const std::vector<uint8_t> & bytes);
    void FireOnClosedEvent();

    void BindDelegate(Delegate * delegate);
    void UnbindDelegate();

protected:
    Connection();
    virtual ~Connection() {}

private:
    Delegate * delegate_;
};


}  // namespace codebase


#endif  // YGG_CODEBASE_CONNECTION_H_
