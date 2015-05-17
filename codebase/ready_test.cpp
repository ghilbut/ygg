#include <gmock/gmock.h>

#include "ready.h"
#include "test/test_fake.h"
#include "test/test_mock.h"


using ::testing::_;
using ::testing::Invoke;


namespace codebase {


class DummyProxy : public Object<DummyProxy> {
public:
    static DummyProxy::Ptr New(
        Session::Ptr & session
        , const std::string & text
        , test::LifeCycleMock * mock = nullptr) {

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
        , test::LifeCycleMock * mock)
        : Object(), session_(session), text_(text), mock_(mock) {

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
    test::LifeCycleMock * mock_;
};


class DummyReady : public Ready<DummyProxy> {
public:
    explicit DummyReady(test::LifeCycleMock * mock = nullptr) 
        : mock_(mock) {}

    virtual DummyProxy::Ptr NewProxy(Session * session, const std::string & text) const {
        Session::Ptr ptr(session);
        return DummyProxy::New(ptr, text, mock_);
    }

private:
    test::LifeCycleMock * mock_;
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

    Session::Ptr session(test::FakeSession::New());

    DummyReady ready;
    ready.SetSession(session);
    ASSERT_TRUE(ready.HasSession(session));
}


TEST(ReadyTest, test_remove_session_when_disconnected) {

    test::LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(1);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(1);
    
    Session::Ptr session(test::FakeSession::New(&life_cycle_mock));

    DummyReady ready;
    ready.SetSession(session);

    session->FireOnClosedEvent();
    ASSERT_FALSE(ready.HasSession(session));
}

TEST(ReadyTest, test_remove_session_when_invalid_text_received_from_session) {

    test::LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(1);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(1);

    DummyReady ready;
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_)).Times(1);

    Session::Ptr session(test::FakeSession::New(&life_cycle_mock));

    ready.SetSession(session);
    session->FireOnTextEvent("");
    ASSERT_FALSE(ready.HasSession(session));
}

TEST(ReadyTest, test_remove_session_when_valid_text_received_from_session) {

    test::LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(1);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(1);

    DummyReady ready;
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_)).Times(1);

    Session::Ptr session(test::FakeSession::New(&life_cycle_mock));

    ready.SetSession(session);
    session->FireOnTextEvent("test");
    ASSERT_FALSE(ready.HasSession(session));
}


void CheckNullProxy(DummyProxy::Ptr & proxy) {
    ASSERT_TRUE(proxy == nullptr);
}

TEST(ReadyTest, test_passing_null_proxy_to_delegate_when_invalid_text_received_from_session) {

    DummyReady ready;
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_))
        .Times(1)
        .WillOnce(Invoke(CheckNullProxy));

    Session::Ptr session(test::FakeSession::New());

    ready.SetSession(session);
    session->FireOnTextEvent("");
}

TEST(ReadyTest, test_check_proxy_life_cycle_when_invalid_text_received_from_session) {

    test::LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(0);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(0);

    DummyReady ready(&life_cycle_mock);
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_)).Times(1);

    Session::Ptr session(test::FakeSession::New());

    ready.SetSession(session);
    session->FireOnTextEvent("");
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

    Session::Ptr session(test::FakeSession::New());

    ready.SetSession(session);
    session->FireOnTextEvent(expected_text);
}

TEST(ReadyTest, test_check_proxy_life_cycle_when_valid_text_received_from_session) {

    test::LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(1);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(1);

    DummyReady ready(&life_cycle_mock);
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_)).Times(1);

    Session::Ptr session(test::FakeSession::New());

    ready.SetSession(session);
    session->FireOnTextEvent("text");
}


}  // namespace codebase
