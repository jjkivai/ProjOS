#include <cstddef>
#include "ISR.hpp"
#include "IDT.hpp"
#include "../kernelGDT.hpp"
#include "../../drivers/ports.hpp"
#include "../../drivers/display.hpp"
#define KERNEL_CS 0x08
#define KERNEL_DS 0x10
namespace ISR {
    void load_gates() {
        IDT::set_gate(0, (uint32_t)isr0, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(1, (uint32_t)isr1, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(2, (uint32_t)isr2, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(3, (uint32_t)isr3, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(4, (uint32_t)isr4, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(5, (uint32_t)isr5, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(6, (uint32_t)isr6, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(7, (uint32_t)isr7, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(8, (uint32_t)isr8, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(9, (uint32_t)isr9, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(10, (uint32_t)isr10, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(11, (uint32_t)isr11, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(12, (uint32_t)isr12, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(13, (uint32_t)isr13, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(14, (uint32_t)isr14, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(15, (uint32_t)isr15, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(16, (uint32_t)isr16, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(17, (uint32_t)isr17, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(18, (uint32_t)isr18, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(19, (uint32_t)isr19, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(20, (uint32_t)isr20, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(21, (uint32_t)isr21, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(22, (uint32_t)isr22, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(23, (uint32_t)isr23, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(24, (uint32_t)isr24, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(25, (uint32_t)isr25, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(26, (uint32_t)isr26, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(27, (uint32_t)isr27, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(28, (uint32_t)isr28, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(29, (uint32_t)isr29, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(30, (uint32_t)isr30, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(31, (uint32_t)isr31, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        
        
        /* Program the Programmable Interrupt Controller (PIC)*/
        port_byte_out(0x20, 0x11);
        port_byte_out(0xA0, 0x11);
        port_byte_out(0x21, 0x20);
        port_byte_out(0xA1, 0x28);
        port_byte_out(0x21, 0x04);
        port_byte_out(0xA1, 0x02);
        port_byte_out(0x21, 0x01);
        port_byte_out(0xA1, 0x01);
        port_byte_out(0x21, 0x0);
        port_byte_out(0xA1, 0x0);

        IDT::set_gate(32, (uint32_t)irq0, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(33, (uint32_t)irq1, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(34, (uint32_t)irq2, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(35, (uint32_t)irq3, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(36, (uint32_t)irq4, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(37, (uint32_t)irq5, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(38, (uint32_t)irq6, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(39, (uint32_t)irq7, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(40, (uint32_t)irq8, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(41, (uint32_t)irq9, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(42, (uint32_t)irq10, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(43, (uint32_t)irq11, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(44, (uint32_t)irq12, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(45, (uint32_t)irq13, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(46, (uint32_t)irq14, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
        IDT::set_gate(47, (uint32_t)irq15, KERNEL_CS, IDT::RING0| IDT::INTERRUPT_GATE_32BIT);
    }

    void Init() {
        load_gates();
        for(int i = 0; i < 47; i++) {
            IDT::enable_interrupt(i);
        }
        IDT::disable_interrupt(80);
    }
    Handler handlers[256];
    void register_handler(uint8_t interrupt, Handler handler) {
        handlers[interrupt] = handler;
        IDT::enable_interrupt(interrupt);
    }
    extern "C" void interrupt_handler(Register *reg) {
        Display display;
        if(handlers[reg->interrupt] != NULL) {
            handlers[reg->interrupt](reg);
        } else {
            display.print_string("Unhandled interrupt: %s\n", Exception_Messages[reg->interrupt]);
        }
        reg->eip += 3;
    }
    extern "C" void interrupt_request_handler(Register *reg) {

        if(reg->interrupt >= 40) {
            port_byte_out(0xA0, 0x20);
        }
        port_byte_out(0x20, 0x20);

        if(handlers[reg->interrupt] != NULL) {
            handlers[reg->interrupt](reg);
        }
    }
}