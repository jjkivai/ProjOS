#include "kernelGDT.hpp"

namespace GDT {
    Entry get_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
        Entry entry;
        entry.limit_low = (limit & 0xFFFF); // The lower 16 bits of the limit.
        entry.base_low = (base & 0xFFFF); // The lower 16 bits of the base.
        entry.base_middle = (base >> 16) & 0xFF; // The next 8 bits of the base.
        entry.access = access; // Access flags, determine what ring this segment can be used in.
        entry.granularity = ((limit >> 16) & 0x0F); // Granularity flags, determine the scaling of the limit.
        entry.granularity |= (flags & 0xF0); 
        entry.base_high = (base >> 24) & 0xFF; // The last 8 bits of the base.
        return entry;
    }

    Entry Table[] = {
        get_entry(0, 0, 0, 0), // Null segment.
        get_entry(0, 0xFFFFF, ACCESS_PRESENT|ACCESS_RING0|ACCESS_CODE_SEGMENT|ACCESS_CODE_READABLE, GRANULARITY_4K|FLAG_32BIT),
        get_entry(0, 0xFFFFF, ACCESS_PRESENT|ACCESS_RING0| ACCESS_DATA_SEGMENT|ACCESS_DATA_WRITEABLE, GRANULARITY_4K|FLAG_32BIT),
    };
    Descriptor descriptor = {
        sizeof(Table) - 1,
        Table
    };
    void Init() {
        __asm__ volatile("lgdt %0" : : "m"(descriptor));
    }
} // namespace GDT
