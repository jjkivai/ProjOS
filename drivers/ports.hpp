#pragma once
#include <stdint.h>

extern "C" {

    void port_byte_out(uint16_t port, uint8_t data);
    uint8_t port_byte_in(uint16_t port);

    bool Disk_GetDriveParameters(uint16_t drive, 
    uint8_t *driveType,
    uint16_t *cylinders, 
    uint8_t *sectors, 
    uint8_t *heads);

    bool Disk_Reset(uint16_t drive);

    bool Disk_ReadSectors(uint16_t drive,
    uint16_t cylinder,
    uint16_t sector,
    uint16_t head,
    uint16_t count,
    void* buffer);

    void doNothing();

}