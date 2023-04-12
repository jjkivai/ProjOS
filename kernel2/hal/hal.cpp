#include "hal.hpp"
#include "../i686/gdt.hpp"
#include "../i686/idt.hpp"
#include "../i686/isr.hpp"

void HAL_Initialize()
{
    //i686_GDT_Initialize();
    i686_IDT_Initialize();
    i686_ISR_Initialize();
}
