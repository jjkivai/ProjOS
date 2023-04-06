#pragma once
#include <stdint.h>
# define MEMORY_MIN 0x000500
# define MEMORY_MAX 0x080000

#define MEMORY_FAT_ADDRESS ((void*)0x20000)
#define MEMORY_FAT_SIZE 0x10000

#define MEMORY_KERNEL ((void*)0x30000)
#define KERNEL_SIZE 0x10000

#define MEMORY_KERNEL_ADDRESS ((void*)0x100000)


extern "C" {
    void *memcpy(void *dest, const void *src, uint16_t count);
    void *memset(void *dest, uint8_t val, uint16_t count);
    int memcmp(const void *s1, const void *s2, uint16_t count);
}