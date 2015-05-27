#ifndef YGG_TEST_FAKE_H_
#define YGG_TEST_FAKE_H_

#include "mock.h"
#include "codebase/connection.h"
#include "codebase/session.h"


using namespace codebase;


namespace test {


class FakeConnection;
class FakeSession;


class FakeConnection : public Connection {
public:
    FakeConnection(LifeCycleMock * mock = nullptr);
    FakeConnection(Session * session, LifeCycleMock * mock = nullptr);
    ~FakeConnection();

    virtual bool Open();
    virtual size_t SendText(const std::string & text) const;
    virtual size_t SendBinary(const std::vector<uint8_t> & bytes) const;
    virtual void Close();

    Session::Ptr session() const;

private:
    FakeSession * fake_;
    Session::Weak session_;
    LifeCycleMock * mock_;
};


class FakeSession : public Session {
public:
    FakeSession(LifeCycleMock * mock = nullptr);
    FakeSession(Connection * conn, LifeCycleMock * mock = nullptr);
    ~FakeSession();

    virtual size_t SendText(const std::string & text) const;
    virtual size_t SendBinary(const std::vector<uint8_t> & bytes) const;
    virtual void Close();

    Connection::Ptr conn() const;

private:
    FakeConnection * fake_;
    Connection::Weak conn_;
    LifeCycleMock * mock_;
};


}  // namespace test


#endif  // YGG_TEST_FAKE_H_
