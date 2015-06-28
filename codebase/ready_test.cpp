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
        Connection::Ptr & conn
        , const std::string & text
        , LifeCycleMock * mock = nullptr) {

        if (text.empty()) {
            return nullptr;
        }
        return new DummyProxy(conn, text, mock);
    }

    const char * text() const {
        return text_.c_str();
    }

private:
    DummyProxy(
        Connection::Ptr & conn
        , const std::string & text
        , LifeCycleMock * mock)
        : Object(this), conn_(conn), text_(text), mock_(mock) {

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
    Connection::Ptr conn_;
    const std::string text_;
    LifeCycleMock * mock_;
};


class DummyReady : public Ready<DummyProxy> {
public:
    explicit DummyReady(LifeCycleMock * mock = nullptr) 
        : mock_(mock) {}

    virtual DummyProxy::Ptr NewProxy(Connection * conn, const std::string & text) const {
        Connection::Ptr ptr(conn);
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


TEST(ReadyTest, test_set_conn) {

    Connection::Ptr conn(FakeConnection::New());

    DummyReady ready;
    ready.SetConnection(conn);
    ASSERT_TRUE(ready.HasConnection(conn));
}


TEST(ReadyTest, test_remove_conn_when_disconnected) {

    LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(1);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(1);
    
    Connection::Ptr conn(FakeConnection::New(&life_cycle_mock));

    DummyReady ready;
    ready.SetConnection(conn);

    conn->FireOnClosedEvent();
    ASSERT_FALSE(ready.HasConnection(conn));
}

TEST(ReadyTest, test_remove_conn_when_invalid_text_received_from_conn) {

    LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(1);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(1);

    DummyReady ready;
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_)).Times(0);

    Connection::Ptr conn(FakeConnection::New(&life_cycle_mock));

    ready.SetConnection(conn);
    conn->FireOnTextEvent("");
    ASSERT_FALSE(ready.HasConnection(conn));
}

TEST(ReadyTest, test_remove_conn_when_valid_text_received_from_conn) {

    LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(1);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(1);

    DummyReady ready;
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_)).Times(1);

    Connection::Ptr conn(FakeConnection::New(&life_cycle_mock));

    ready.SetConnection(conn);
    conn->FireOnTextEvent("test");
    ASSERT_FALSE(ready.HasConnection(conn));
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

TEST(ReadyTest, test_passing_non_null_proxy_to_delegate_when_valid_text_received_from_conn) {

    const char * expected_text = "text";
    DummyProxyChecker checker(expected_text);
    auto invoke = Invoke(&checker, &DummyProxyChecker::Check);

    DummyReady ready;
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_))
        .Times(1)
        .WillOnce(invoke);

    Connection::Ptr conn(FakeConnection::New());

    ready.SetConnection(conn);
    conn->FireOnTextEvent(expected_text);
}

TEST(ReadyTest, test_check_proxy_life_cycle_when_valid_text_received_from_conn) {

    LifeCycleMock life_cycle_mock;
    EXPECT_CALL(life_cycle_mock, constructed()).Times(1);
    EXPECT_CALL(life_cycle_mock, destructed()).Times(1);

    DummyReady ready(&life_cycle_mock);
    ServerMock server_mock(ready);
    EXPECT_CALL(server_mock, OnReady(_)).Times(1);

    Connection::Ptr conn(FakeConnection::New());

    ready.SetConnection(conn);
    conn->FireOnTextEvent("text");
}


}  // namespace codebase
