#include <stdint.h>
#include "../drivers/display.hpp"
#include "../drivers/memory.hpp"
#include "kernelGDT.hpp"
#include "Interrupts/IDT.hpp"
#include "Interrupts/ISR.hpp"

extern uint8_t __bss_start;
extern uint8_t __end;

extern "C" {

    void start(uint16_t bootDrive) {
        memory::memset(&__bss_start, 0, (&__end) - (&__bss_start));
        GDT::Init();
        IDT::Init();
        ISR::Init();
        //clrscr();

        Display display = Display::get_instance();
        // Two newlines for lines printed from loader
        display.print_newline();
        display.print_newline();
        // Kernel starts here
        display.print_string("Hello world from kernel!!!\n");
        display.print_string("Boot drive:%d\n", bootDrive);
        int a = 0;
        int b = 1;
        int c = b/a;
        display.print_string("Interrupt handled\n");
        display.print_string("Exiting kernel...\n");

        return;
    }
}