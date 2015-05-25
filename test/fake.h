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
    ~FakeConnection();

    virtual bool Open();
    virtual size_t SendText(const std::string & text) const;
    virtual size_t SendBinary(const std::vector<uint8_t> & bytes) const;
    virtual void Close();

    Session * GetSession() const;

private:
    LifeCycleMock * mock_;
    Session::Ptr session_;
};


class FakeSession : public Session {
public:
    FakeSession(LifeCycleMock * mock = nullptr);
    ~FakeSession();

    virtual size_t SendText(const std::string & text) const;
    virtual size_t SendBinary(const std::vector<uint8_t> & bytes) const;
    virtual void Close();

private:
    LifeCycleMock * mock_;
};


}  // namespace test


#endif  // YGG_TEST_FAKE_H_
