#include <gmock/gmock.h>

#include "ready.h"
#include "test/fake.h"
#include "test/mock.h"


using ::testing::_;
using ::testing::Invoke;

using namespace test;


namespace codebase {


class DummyProxy : public Object<DummyProxy> {
public:
    static DummyProxy::Ptr New(
        Session::Ptr & session
        , const std::string & text
        , LifeCycleMock * mock = nullptr) {

        if (text.empty()) {
            return nullptr;
        }
        return new DummyProxy(session, text, mock);
    }

    const char * text() const {
        return text_.c_str();
    }

private:
    DummyProxy(
        Session::Ptr & session
        , const std::string & text
        , LifeCycleMock * mock)
        : Object(this), session_(session), text_(text), mock_(mock) {

        if (mock_ != nullptr) {
            mock_->constructed();
        }
    }

    ~DummyProxy() {
        if (mock_ != nullptr) {
            mock_->destructed();
        }
    }

private:
    Session::Ptr session_;
    const std::string text_;
    LifeCycleMock * mock_;
};


class DummyReady : public Ready<DummyProxy> {
public:
    explicit DummyReady(LifeCycleMock * mock = nullptr) 
        : mock_(mock) {}

    virtual DummyProxy::Ptr NewProxy(Session * session, const std::string & text) const {
        Session::Ptr ptr(session);
        return DummyProxy::New(ptr, text, mock_);
    }

private:
    LifeCycleMock * mock_;
};


class ServerMock : public DummyReady::Delegate {
public:
    ServerMock(DummyReady & ready)
        : ready_(ready) {
        ready_.BindDelegate(this);
    }

    MOCK_METHOD1(OnReady, void(DummyProxy::Ptr&));

private:
    DummyReady & ready_;
};


TEST(ReadyTest, test_set_session) {

    Session::Ptr session(new FakeSession());

    DummyReady ready;
    ready.SetSession(session);
    ASSERT_TRUE(ready.HasSession(session));
}


TEST(ReadyTest, test_remove_session_when_disconnected) {

    LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(1);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(1);
    
    Session::Ptr session(new FakeSession(&life_cycle_mock));

    DummyReady ready;
    ready.SetSession(session);

    session->FireOnClosedEvent();
    ASSERT_FALSE(ready.HasSession(session));
}

TEST(ReadyTest, test_remove_session_when_invalid_text_received_from_session) {

    LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(1);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(1);

    DummyReady ready;
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_)).Times(0);

    Session::Ptr session(new FakeSession(&life_cycle_mock));

    ready.SetSession(session);
    session->FireOnTextEvent("");
    ASSERT_FALSE(ready.HasSession(session));
}

TEST(ReadyTest, test_remove_session_when_valid_text_received_from_session) {

    LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(1);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(1);

    DummyReady ready;
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_)).Times(1);

    Session::Ptr session(new FakeSession(&life_cycle_mock));

    ready.SetSession(session);
    session->FireOnTextEvent("test");
    ASSERT_FALSE(ready.HasSession(session));
}


class DummyProxyChecker {
public:
    DummyProxyChecker(const std::string & expected)
        : expected_(expected) {
        // nothing
    }

    void Check(DummyProxy::Ptr & proxy) {
        ASSERT_TRUE(proxy != nullptr);
        ASSERT_STREQ(expected_.c_str(), proxy->text());
    }

private:
    const std::string & expected_;
};

TEST(ReadyTest, test_passing_non_null_proxy_to_delegate_when_valid_text_received_from_session) {

    const char * expected_text = "text";
    DummyProxyChecker checker(expected_text);
    auto invoke = Invoke(&checker, &DummyProxyChecker::Check);

    DummyReady ready;
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_))
        .Times(1)
        .WillOnce(invoke);

    Session::Ptr session(new FakeSession());

    ready.SetSession(session);
    session->FireOnTextEvent(expected_text);
}

TEST(ReadyTest, test_check_proxy_life_cycle_when_valid_text_received_from_session) {

    LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(1);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(1);

    DummyReady ready(&life_cycle_mock);
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_)).Times(1);

    Session::Ptr session(new FakeSession());

    ready.SetSession(session);
    session->FireOnTextEvent("text");
}


}  // namespace codebase
