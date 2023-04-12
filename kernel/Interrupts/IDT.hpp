#pragma once
#include <stdint.h>

namespace IDT {
    typedef enum FLAGS{
        TASK_GATE = 0x05,
        INTERRUPT_GATE = 0x06,
        TRAP_GATE = 0x07,
        INTERRUPT_GATE_32BIT = 0x0E,
        TRAP_GATE_32BIT = 0x0F,

        PRESENT = 0x80,

        RING0 = 0x00,
        RING1 = 0x32,
        RING2 = 0x64,
        RING3 = 0x96,
    } FLAGS;
    void Init();
    void set_gate(uint8_t interrupt, uint32_t base, uint16_t selector, uint8_t flags);
    void enable_interrupt(uint8_t interrupt);
    void disable_interrupt(uint8_t interrupt);
}