#include "global.hpp"
#include <memory.h>
#include <string>

const char* get_hex_string(const uint8_t* data, uint32_t len, uint64_t& buf_siz)
{
    uint8_t step = 3;
    uint64_t buffer_size = step * len + 1;
    buf_siz = buffer_size;
    char* buffer = new char[buffer_size];
    ::memset(buffer, 0, buffer_size);
    uint32_t cnt = 0;
    uint32_t buf_cursor = 0;
    while(cnt < len)
    {
        std::sprintf(buffer + buf_cursor, "%02X|", *(data + cnt));
        ++cnt;
        buf_cursor += step;
    }
    return buffer;
}