#pragma once
#include <stdint.h>

extern "C" {

    void port_byte_out(uint16_t port, uint8_t data);
    uint8_t port_byte_in(uint16_t port);

    extern "C" bool Disk_GetDriveParameters(uint8_t drive, 
    uint8_t *driveType,
    uint16_t *cylinders, 
    uint16_t *sectors, 
    uint16_t *heads);

    bool Disk_Reset(uint8_t drive);

    bool Disk_ReadSectors(uint8_t drive,
    uint16_t cylinder,
    uint16_t sector,
    uint16_t head,
    uint8_t count,
    void* buffer);

    void doNothing();

}