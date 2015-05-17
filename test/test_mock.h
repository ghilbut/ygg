#ifndef YGG_TEST_MOCK_H_
#define YGG_TEST_MOCK_H_

#include <gmock/gmock.h>


namespace test {


class LifeCycleMock {
public:
    MOCK_CONST_METHOD0(constructed, void());
    MOCK_CONST_METHOD0(destructed, void());

    ~LifeCycleMock() {}
};


}  // namespace test


#endif  // YGG_TEST_MOCK_H_
