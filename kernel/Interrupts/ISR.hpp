#pragma once
#include <stdint.h>

namespace ISR {
    static const char* const Exception_Messages[] {
        "DivisionByZero",
        "SingleStepInterrupt",//used for debugging
        "NonMaskableInterrupt",
        "BreakPoint",
        "Overflow",
        "OutOfBounds",
        "InvalidOpcode",
        "NoCoprocessor",

        "DoubleFault",
        "SegmentOverrun",
        "InvalidTaskStateSegment",
        "SegmentNotPresent",
        "StackSegmentError",
        "GeneralProtectionFault",
        "PageFault",
        "Reserved",

        "FloatingPointException",
        "AlignmentCheck",
        "MachineCheck",
        "FloatingPointException",
        "VirtualizationException",
        "ControlProtectionException",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",

        "Reserved",
        "Reserved",
        "Reserved",
        "HypervisorInjectionException",
        "VMMCommunicationException"
        "SecurityException",
        "Reserved"
    };
    typedef struct Register {
        uint32_t ds;
        uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
        uint32_t interrupt, error_code;
        uint32_t eip, cs, eflags, useresp, ss;
    } Register;
    typedef void(*Handler)(Register* reg);

    void Init();
    void load_gates();
    void register_handler(uint8_t interrupt, Handler handler);
    extern "C" void interrupt_handler(Register* reg);
    extern "C" void interrupt_request_handler(Register* reg);

    /* ISRs reserved for ISR exceptions*/
    extern "C" void isr0();
    extern "C" void isr1();
    extern "C" void isr2();
    extern "C" void isr3();
    extern "C" void isr4();
    extern "C" void isr5();
    extern "C" void isr6();
    extern "C" void isr7();
    extern "C" void isr8();
    extern "C" void isr9();
    extern "C" void isr10();
    extern "C" void isr11();
    extern "C" void isr12();
    extern "C" void isr13();
    extern "C" void isr14();
    extern "C" void isr15();
    extern "C" void isr16();
    extern "C" void isr17();
    extern "C" void isr18();
    extern "C" void isr19();
    extern "C" void isr20();
    extern "C" void isr21();
    extern "C" void isr22();
    extern "C" void isr23();
    extern "C" void isr24();
    extern "C" void isr25();
    extern "C" void isr26();
    extern "C" void isr27();
    extern "C" void isr28();
    extern "C" void isr29();
    extern "C" void isr30();
    extern "C" void isr31();

    /* IRQs reserved for ISR exceptions*/
    extern "C" void irq0();
    extern "C" void irq1();
    extern "C" void irq2();
    extern "C" void irq3();
    extern "C" void irq4();
    extern "C" void irq5();
    extern "C" void irq6();
    extern "C" void irq7();
    extern "C" void irq8();
    extern "C" void irq9();
    extern "C" void irq10();
    extern "C" void irq11();
    extern "C" void irq12();
    extern "C" void irq13();
    extern "C" void irq14();
    extern "C" void irq15();

    /* Requests for hardware exceptions*/
    #define IRQ0 32
    #define IRQ1 33
    #define IRQ2 34
    #define IRQ3 35
    #define IRQ4 36
    #define IRQ5 37
    #define IRQ6 38
    #define IRQ7 39
    #define IRQ8 40
    #define IRQ9 41
    #define IRQ10 42
    #define IRQ11 43
    #define IRQ12 44
    #define IRQ13 45
    #define IRQ14 46
    #define IRQ15 47
}
