#include "util.h"


static bool is_big_endian(void) {
    static const int n = 1;
    return ((char *)&n)[0] == 0;
}


namespace codebase {
namespace net {

uint32_t DecodeSize(uint32_t size) {

    if (is_big_endian()) {
        return size;
    }

    uint32_t ret;
    char* tmp = reinterpret_cast<char*>(&ret);
    const char* psize = reinterpret_cast<const char*>(&size);
    tmp[0] = psize[3];
    tmp[1] = psize[2];
    tmp[2] = psize[1];
    tmp[3] = psize[0];
    return ret;
}

void EncodeSize(uint32_t size, char buffer[4]) {

    const char* tmp = reinterpret_cast<const char*>(&size);
    if (is_big_endian()) {
        buffer[0] = tmp[0];
        buffer[1] = tmp[1];
        buffer[2] = tmp[2];
        buffer[3] = tmp[3];
    }
    else {
        buffer[0] = tmp[3];
        buffer[1] = tmp[2];
        buffer[2] = tmp[1];
        buffer[3] = tmp[0];
    }
}

}  // namespace net
}  // namespace codebase