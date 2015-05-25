#include <gmock/gmock.h>

#include "mock.h"


namespace test {


TEST(MockTest, test_lifcyclemock) {
    LifeCycleMock mock;
    EXPECT_CALL(mock, constructed());
    EXPECT_CALL(mock, destructed());
    mock.constructed();
    mock.destructed();
}

TEST(MockTest, test) {

}


}  // namespace test
