#ifndef YGG_TEST_MOCK_H_
#define YGG_TEST_MOCK_H_

#include "codebase/connection.h"
#include <gmock/gmock.h>


using namespace codebase;


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


}  // namespace test


#endif  // YGG_TEST_MOCK_H_
