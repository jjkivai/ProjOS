
#include "util.h"
#include "drivers/idt.h"
#include "drivers/isr.h"
#include "drivers/display.h"

void main()
{
    clear_screen();
    print_string("Installing ISRs\n");
    isr_install();
    print_string("Enabling external interrupts.\n");
    __asm__("sti");

    print_string("Welcome to ProjOS\n");
    print_string(">>> ");
}
