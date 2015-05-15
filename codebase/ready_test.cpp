#include <gmock/gmock.h>

#include "ready.h"


namespace codebase {


class ReadyTest : public ::testing::Test {
};


class FakeSession : public Session {
public:
    FakeSession() {}
    ~FakeSession() {}

    virtual size_t SendText(const std::string & text) const {
        return text.length();
    }

    virtual size_t SendBinary(const uint8_t bytes[], size_t size) const {
        return size;
    }

    virtual void Close() {
        // nothing
    }

private:
};


class DummyProxy {
public:
    static DummyProxy * New(const std::string & text, Session * session) {
        return new DummyProxy(session);
    }

private:
    DummyProxy(Session * session) : session_(session) {}

private:
    Session * session_;
};


typedef Ready<DummyProxy> DummyReady;

class ServerMock : public DummyReady::Delegate {
public:
    ServerMock(DummyReady & ready) : ready_(ready) {}

    MOCK_METHOD1(OnReady, void(DummyProxy*));

private:
    DummyReady & ready_;
};


TEST_F(ReadyTest, test_set_session) {

    FakeSession session;

    DummyReady ready;
    ready.SetSession(&session);
    ASSERT_TRUE(ready.HasSession(&session));
}

TEST_F(ReadyTest, test_remove_session_when_disconnected) {

    FakeSession session;

    DummyReady ready;
    ready.SetSession(&session);

    session.FireOnClosedEvent();
    ASSERT_FALSE(ready.HasSession(&session));
}


}  // namespace codebase
