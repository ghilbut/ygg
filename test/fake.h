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
    static Connection::Ptr New(LifeCycleMock * mock = nullptr);
    static Connection::Ptr New(Session * session, LifeCycleMock * mock = nullptr);
    
    virtual bool Open();
    virtual size_t SendText(const std::string & text) const;
    virtual size_t SendBinary(const std::vector<uint8_t> & bytes) const;
    virtual void Close();

    Session::Ptr session() const;

private:
    friend FakeSession;
    FakeConnection(LifeCycleMock * mock);
    FakeConnection(Session * session, LifeCycleMock * mock = nullptr);
    ~FakeConnection();

private:
    FakeSession * fake_;
    Session::Weak session_;
    LifeCycleMock * mock_;
};


class FakeSession : public Session {
public:
    static Session::Ptr New(LifeCycleMock * mock = nullptr);
    static Session::Ptr New(Connection * conn, LifeCycleMock * mock = nullptr);

    virtual size_t SendText(const std::string & text) const;
    virtual size_t SendBinary(const std::vector<uint8_t> & bytes) const;
    virtual void Close();

    Connection::Ptr conn() const;

private:
    friend FakeConnection;
    FakeSession(LifeCycleMock * mock);
    FakeSession(Connection * conn, LifeCycleMock * mock = nullptr);
    ~FakeSession();

private:
    FakeConnection * fake_;
    Connection::Weak conn_;
    LifeCycleMock * mock_;
};


}  // namespace test


#endif  // YGG_TEST_FAKE_H_
