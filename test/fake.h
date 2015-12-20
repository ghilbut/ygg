#ifndef YGG_TEST_FAKE_H_
#define YGG_TEST_FAKE_H_

#include "mock.h"
#include "core/connection.h"


namespace ygg {
namespace test {


class FakeConnection : public Connection {
public:
    static Connection::Ptr New(LifeCycleMock * mock = nullptr);
    static Connection::Ptr New(Connection * conn, LifeCycleMock * mock = nullptr);

    virtual size_t SendText(const std::string & text) const;
    virtual size_t SendBinary(const std::vector<uint8_t> & bytes) const;
    virtual void Close();

    Connection::Ptr conn() const;

private:
    FakeConnection(LifeCycleMock * mock);
    FakeConnection(Connection * conn, LifeCycleMock * mock = nullptr);
    ~FakeConnection();

private:
    FakeConnection * fake_;
    WeakPtr<Connection> conn_;
    //Connection::Weak conn_;
    LifeCycleMock * mock_;
};


}  // namespace test
}  // namesapce ygg


#endif  // YGG_TEST_FAKE_H_
