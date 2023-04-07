#include <stdint.h>
#include <stddef.h>
#include "../drivers/display.hpp"
#include "../drivers/ports.hpp"
#include "../drivers/memory.hpp"
#include "../drivers/disk.hpp"
#include "fat.hpp"

extern "C" {
    uint8_t* KernelLoadBuffer = (uint8_t*)MEMORY_LOAD_KERNEL;
    uint8_t* Kernel = (uint8_t*)MEMORY_KERNEL_ADDR;

    typedef void (*KernelStart)(int);

    void __attribute__((cdecl)) _start(uint16_t bootDrive)
    {
        Display display;
        display.clear_screen();

        Disk::DISK disk;
        if (!Disk::Init(&disk, bootDrive))
        {
            display.print_string("Disk init error\r\n");
        }

        if (!FAT::Initialize(&disk))
        {
            display.print_string("FAT init error\r\n");
        }

        // load kernel
        FAT::FILE* fd = FAT::Open(&disk, "/kernel.bin");
        if(fd!= NULL) {
            display.print_string("Kernel has been found\n");
        }
        uint32_t read;
        uint8_t* kernelBuffer = Kernel;
        while ((read = FAT::Read(&disk, fd, MEMORY_LOAD_SIZE, KernelLoadBuffer)))
        {
            memory::memcpy(kernelBuffer, KernelLoadBuffer, read);
            kernelBuffer += read;
        }
        display.print_string("Kernel has been loaded into memory\n");
        FAT::Close(fd);

        // execute kernel
        display.print_string("Starting kernel\n");
        KernelStart kernelStart = (KernelStart)Kernel;
        kernelStart(bootDrive);

        display.print_string("Kernel has finished running\n"); // once kernel properly exits, we should never get here
        display.print_string("Halting\n");
        asm volatile("hlt");


    } 
}