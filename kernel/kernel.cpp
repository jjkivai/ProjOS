#include <stdint.h>
#include "../drivers/display.hpp"
#include "../drivers/memory.hpp"

extern uint8_t __bss_start;
extern uint8_t __end;

extern "C" {

    void start(uint16_t bootDrive)
    {
        memory::memset(&__bss_start, 0, (__end) - (__bss_start));

        //clrscr();
        Display display;
        // display.clear_screen();
        display.print_newline();
        display.print_newline();
        display.print_string("Hello world from kernel!!!\n");
        display.print_string("Boot drive:%d\n", bootDrive);
        // display.print_newline();
        // for(;;);
        return;
    }
}