#include <stdint.h>

#include "../drivers/display.hpp"
#include "../drivers/ports.hpp"
#include "../drivers/memory.hpp"
#include "../drivers/disk.hpp"

#include "fat.hpp"

extern "C" {
    void start(uint16_t bootDrive) {
        Display Display;
        Display.clear_screen();
        Display.print_string("Setting Up params\n");

        DISK disk;
        if(!Disk_Init(&disk, bootDrive)) {
            Display.print_string("Failed to initialize disk!\n");
            return;
        } else {
            Display.print_string("Disk initialized!\n");
        }
        if(!FAT_Init(&disk)) {
            Display.print_string("Failed to initialize FAT!\n");
            return;
        } else {
            Display.print_string("FAT initialized!\n");
        }
    } 
}