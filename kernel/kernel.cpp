#include <stdint.h>
#include "../drivers/display.hpp"
#include "../drivers/memory.hpp"

extern uint8_t __bss_start;
extern uint8_t __end;

extern "C" {

    void __attribute__((section(".entry"))) start(uint16_t bootDrive)
    {
        memory::memset(&__bss_start, 0, (&__end) - (&__bss_start));

        //clrscr();
        Display display;
        display.clear_screen();
        display.print_string("Hello world from kernel!!!\n");

    }
}