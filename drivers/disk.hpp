#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "ports.hpp"

namespace Disk {


    typedef struct __attribute__((packed)) DISK{
        uint8_t id;
        uint16_t cylinders;
        uint16_t sectors;
        uint16_t heads;
    } DISK;

    bool Init(DISK *disk, uint8_t id);
    bool Read(DISK *disk, uint32_t lba, uint8_t sectors, void* buffer);
    
}
