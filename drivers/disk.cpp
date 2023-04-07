#include "disk.hpp"
// #include "ports.hpp"


extern "C" {
    // Private prototypes
    bool Disk_GetDriveParameters(uint8_t drive, 
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

    
    bool Disk_Init(DISK *disk, uint8_t id) {
        uint8_t driveType;
        uint16_t cylinders, heads, sectors;

        if(!Disk_GetDriveParameters(disk->id, &driveType, &cylinders, &sectors, &heads))
            return false;
        
        disk->id = id;
        disk->cylinders = cylinders;
        disk->heads = heads;
        disk->sectors = sectors;

        return true;
    }
    void Disk_LBA2CHS(DISK *disk, uint32_t lba, uint16_t *cylinder,  uint16_t *sector, uint16_t *head) {
        *sector = (lba % disk->sectors) + 1;
        *cylinder = lba / (disk->heads * disk->sectors);
        *head = (lba / disk->sectors) % disk->heads;
    }
    bool Disk_Read(DISK *disk, uint32_t lba, uint8_t sectors, void* buffer) {
        uint16_t cylinder, head, sector;

        Disk_LBA2CHS(disk, lba, &cylinder, &sector, &head);

        for(int i=0; i < 3; i++) {
            if(Disk_ReadSectors(disk->id, cylinder, sector, head, sectors, buffer))
                return true;
            
            Disk_Reset(disk->id);
        }

        return false;
    }

}