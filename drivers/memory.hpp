#pragma once
#include <stdint.h>
#define MEMORY_MIN          0x00000500
#define MEMORY_MAX          0x00080000

// 0x00000500 - 0x00010500 - FAT driver
#define MEMORY_FAT_ADDR     ((void*)0x20000)
#define MEMORY_FAT_SIZE     0x00010000

#define MEMORY_LOAD_KERNEL  ((void*)0x30000)
#define MEMORY_LOAD_SIZE    0x00010000

// 0x00020000 - 0x00030000 - stage2

// 0x00030000 - 0x00080000 - free

// 0x00080000 - 0x0009FFFF - Extended BIOS data area
// 0x000A0000 - 0x000C7FFF - Video
// 0x000C8000 - 0x000FFFFF - BIOS

#define MEMORY_KERNEL_ADDR  ((void*)0x100000)

extern "C" {
    namespace memory
    {
        void *memcpy(void *dest, const void *src, uint16_t count);
        void *memset(void *dest, uint8_t val, uint16_t count);
        int memcmp(const void *s1, const void *s2, uint16_t count);
    } // namespace memory
    
}