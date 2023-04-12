#include <stdint.h>
#include "stdio.hpp"
#include "memory.hpp"
#include "hal/hal.hpp"

extern uint8_t __bss_start;
extern uint8_t __end;

extern "C" void crash_me();

extern "C" void start(uint16_t bootDrive)
{
    mymemset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Initialize();

    clrscr();

    myprintf("Hello from kernel1!\n");
    int x = 0;
    int y = 5;

    int z = y/x;

    myprintf("Hello from kernel2!\n");
    myprintf("Hello from kernel3!\n");
    myprintf("Hello from kernel4!\n");
    myprintf("Hello from kernel5!\n");

    myprintf("cloding");

    // crash_me();

}
