#ifndef YGG_CODEBASE_NET_UTIL_H_
#define YGG_CODEBASE_NET_UTIL_H_

#include <cstdint>


namespace codebase {
namespace net {

uint32_t DecodeSize(uint32_t size);
void EncodeSize(uint32_t size, char buffer[4]);

}  // namespace net
}  // namespace codebase

#endif  // YGG_CODEBASE_NET_UTIL_H_