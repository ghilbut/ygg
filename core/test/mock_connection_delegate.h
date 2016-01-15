#ifndef CORE_TEST_MOCK_CONNECTION_DELEGATE_H_
#define CORE_TEST_MOCK_CONNECTION_DELEGATE_H_

#include <gmock/gmock.h>
#include "core/connection.h"


namespace ygg {
namespace test {

class MockConnectionDelegate : public Connection::Delegate {
 public:
  MOCK_METHOD2(OnText, void (Connection*, const Text & text));
  MOCK_METHOD2(OnBinary, void (Connection*, const Bytes & bytes));
  MOCK_METHOD1(OnClosed, void (Connection*));
};


}  // namespace test
}  // namespace ygg


#endif  // CORE_TEST_MOCK_CONNECTION_DELEGATE_H_
