
// #include "util.h"
// #include "drivers/idt.h"
// #include "drivers/isr.h"
// #include "drivers/display.h"
#include "cpu/idt.h"
#include "cpu/isr.h"
#include "cpu/timer.h"
#include "drivers/display.h"
// #include "drivers/keyboard.h"

void main()
{
    clear_screen();
    print_string("Installing ISRs\n");
    isr_install();
    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Welcome to ProjOS\n");
    print_string(">>> ");
}
