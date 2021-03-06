#include <gmock/gmock.h>

#include "mock.h"


namespace ygg {
namespace test {


TEST(MockTest, test_lifcyclemock) {
    LifeCycleMock mock;
    EXPECT_CALL(mock, constructed());
    EXPECT_CALL(mock, destructed());
    mock.constructed();
    mock.destructed();
}


}  // namespace test
}  // namespace ygg
