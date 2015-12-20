#ifndef YGG_TEST_VARS_H_
#define YGG_TEST_VARS_H_

#include <string>
#include <vector>
#include <cstdint>


namespace ygg {
namespace test {

std::string GetCtrlJson(const std::string & user,
                        const std::string & endpoint);
std::string GetTargetJson(const std::string & endpoint);

std::string GetRandomString();
std::vector<uint8_t> GetRandomBytes();


}  // namespace test
}  // namespace ygg


#endif  // YGG_TEST_VARS_H_
