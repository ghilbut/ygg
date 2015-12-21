#ifndef YGG_TEST_MOCK_H_
#define YGG_TEST_MOCK_H_

#include "core/connection.h"
#include "server/target/ctrl_ready.h"
#include "server/target/target_ready.h"
#include <gmock/gmock.h>


namespace ygg {


// libtarget/server
typedef Proxy<CtrlDesc> CtrlProxy;
typedef server::target::CtrlReady CtrlReady;
typedef Proxy<TargetDesc> TargetProxy;
typedef server::target::TargetReady TargetReady;


namespace test {


class LifeCycleMock {
public:
    MOCK_CONST_METHOD0(constructed, void());
    MOCK_CONST_METHOD0(destructed, void());

    ~LifeCycleMock() {}
};


class ConnectionDelegateMock : public Connection::Delegate {
public:
    MOCK_METHOD1(OnOpened, void(Connection*));
    MOCK_METHOD2(OnText, void(Connection*, const std::string&));
    MOCK_METHOD2(OnBinary, void(Connection*, const std::vector<uint8_t>&));
    MOCK_METHOD1(OnClosed, void(Connection*));
};


////////////////////////////////////////////////////////////////
// libtarget/server

class MockCtrlReadyDelegate : public CtrlReady::Delegate {
public:
    MOCK_METHOD1(OnProxy, void(CtrlProxy::Ptr&));
};


class MockTargetReadyDelegate : public TargetReady::Delegate {
public:
    MOCK_METHOD1(OnProxy, void(TargetProxy::Ptr&));
};


}  // namespace test
}  // namespace ygg


#endif  // YGG_TEST_MOCK_H_
