#include <stdint.h>

#include "../drivers/display.hpp"

extern "C" {
    void start(uint16_t bootDrive) {
        Display Display;
        Display.clear_screen();
        Display.print_string("Hello, world!");
    } 
}