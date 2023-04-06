#include "memory.hpp"

extern "C" {
    void *memcpy(void *dest, const void *src, uint16_t count) {
        const char *sp = (const char *)src;
        char *dp = (char *)dest;
        for(; count != 0; count--) *dp++ = *sp++;
        return dest;
    }

    void *memset(void *dest, uint8_t val, uint16_t count) {
        char *temp = (char *)dest;
        for( ; count != 0; count--) *temp++ = val;
        return dest;
    }

    int memcmp(const void *s1, const void *s2, uint16_t count) {
            const uint8_t* u8Ptr1 = (const uint8_t *)s1;
            const uint8_t* u8Ptr2 = (const uint8_t *)s2;

            for (uint16_t i = 0; i < count; i++)
                if (u8Ptr1[i] != u8Ptr2[i])
                    return 1;

            return 0;
    }
}