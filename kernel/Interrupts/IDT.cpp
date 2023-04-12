#include "IDT.hpp"

namespace IDT {
    typedef struct Entry {
        uint16_t lower_offset; // The lower 16 bits of the address to jump to when this interrupt fires.
        uint16_t selector; // Kernel segment selector.
        uint8_t zero; // This must always be zero.
        uint8_t flags; // More flags. See documentation.
        uint16_t higher_offset; // The upper 16 bits of the address to jump to.
    } __attribute__((packed)) Entry;

    typedef struct __attribute__((packed)) Descriptor {
        uint16_t size; // The size of the IDT.
        Entry *address; // The address of the IDT.
    }  Descriptor;

    Entry Table[256];
    Descriptor descriptor = {sizeof(Table) - 1, Table};

    void set_gate(uint8_t interrupt, uint32_t base, uint16_t selector, uint8_t flags) {
        Table[interrupt].lower_offset = base & 0xFFFF;
        Table[interrupt].selector = selector;
        Table[interrupt].zero = 0;
        Table[interrupt].flags = flags;
        Table[interrupt].higher_offset = (base >> 16) & 0xFFFF;
    }
    void enable_interrupt(uint8_t interrupt) {
        Table[interrupt].flags |= PRESENT;
    }
    void disable_interrupt(uint8_t interrupt) {
        Table[interrupt].flags &= ~PRESENT;
    }
    void Init() {
        __asm__ volatile("lidt (%0)" : : "r"(&descriptor));
    }

}