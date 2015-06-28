#ifndef YGG_TEST_FAKE_H_
#define YGG_TEST_FAKE_H_

#include "mock.h"
#include "codebase/connection.h"


using namespace codebase;


namespace test {


class FakeConnection;


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
    Connection::Weak conn_;
    LifeCycleMock * mock_;
};


}  // namespace test


#endif  // YGG_TEST_FAKE_H_
