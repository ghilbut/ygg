#include <gmock/gmock.h>

#include "ready.h"
#include "test/test_fake.h"
#include "test/test_mock.h"


namespace codebase {


class DummyProxy {
public:
    static DummyProxy * New(const std::string & text, Session::Ptr & session) {
        return new DummyProxy(session);
    }

private:
    explicit DummyProxy(Session::Ptr & session) : session_(session) {}

private:
    Session::Ptr session_;
};


typedef Ready<DummyProxy> DummyReady;

class ServerMock : public DummyReady::Delegate {
public:
    ServerMock(DummyReady & ready) : ready_(ready) {}

    MOCK_METHOD1(OnReady, void(DummyProxy*));

private:
    DummyReady & ready_;
};


TEST(ReadyTest, test_set_session) {

    Session::Ptr session(test::FakeSession::New());

    DummyReady ready;
    ready.SetSession(session);
    ASSERT_TRUE(ready.HasSession(session));
}

TEST(ReadyTest, test_remove_session_when_disconnected) {

    Session::Ptr session(test::FakeSession::New());

    DummyReady ready;
    ready.SetSession(session);

    session->FireOnClosedEvent();
    ASSERT_FALSE(ready.HasSession(session));
}


}  // namespace codebase
