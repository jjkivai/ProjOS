/*
* This is the Global Descriptor Table for the kernel.
* It is used to define the memory segments of the kernel.
*/

#pragma once
#include <stdint.h>
/* Segment Selector. Used to access code in
the kernel. A priviledged part of the OS
*/
#define KERNEL_CS 0x08
#define KERNEL_DS 0x10

namespace GDT {
    typedef struct Entry {
        uint16_t limit_low; // The lower 16 bits of the limit.
        uint16_t base_low; // The lower 16 bits of the base.
        uint8_t base_middle; // The next 8 bits of the base.
        uint8_t access; // Access flags, determine what ring this segment can be used in.
        uint8_t granularity; // Granularity flags, determine the scaling of the limit.
        uint8_t base_high; // The last 8 bits of the base.
    } __attribute__((packed)) Entry;

    typedef struct Descriptor {
        uint16_t limit; // The size of the GDT.
        Entry* base; // The offset of the GDT.
    } __attribute__((packed)) Descriptor;
    typedef enum {
        ACCESS_CODE_READABLE = 0x02,
        ACCESS_DATA_WRITEABLE = 0x02,

        ACCESS_CODE_CONFORMING = 0x04,
        ACCESS_DATA_EXPAND_DOWN = 0x04,
        ACCESS_DATA_NORMAL = 0x00,

        ACCESS_DATA_SEGMENT = 0x10,
        ACCESS_CODE_SEGMENT = 0x08,

        ACCESS_DESCRIPTOR = 0x80,

        ACCESS_RING0 = 0x00,
        ACCESS_RING1 = 0x20,
        ACCESS_RING2 = 0x40,
        ACCESS_RING3 = 0x60,

        ACCESS_PRESENT = 0x80,
    } ACCESS;

    typedef enum {
        FLAG_64BIT = 0x20,
        FLAG_32BIT = 0x40,
        FLAG_16BIT = 0x00,

        GRANULARITY_1B = 0x00,
        GRANULARITY_4K = 0x80,

    } FLAGS;
    void Init();
}