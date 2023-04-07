#include "disk.hpp"
// #include "ports.hpp"


namespace Disk{
    // Only the asm functions need to be extern "C"
    extern "C" {
        // Private prototypes
        bool GetDriveParameters(uint8_t drive, 
        uint8_t *driveType,
        uint16_t *cylinders, 
        uint16_t *sectors, 
        uint16_t *heads);

        bool Reset(uint8_t drive);

        bool ReadSectors(uint8_t drive,
        uint16_t cylinder,
        uint16_t sector,
        uint16_t head,
        uint8_t count,
        void* buffer);
    }
        bool Init(DISK *disk, uint8_t id) {
            uint8_t driveType;
            uint16_t cylinders, heads, sectors;

            if(!GetDriveParameters(disk->id, &driveType, &cylinders, &sectors, &heads))
                return false;
            
            disk->id = id;
            disk->cylinders = cylinders;
            disk->heads = heads;
            disk->sectors = sectors;

            return true;
        }
        void LBA2CHS(DISK *disk, uint32_t lba, uint16_t *cylinder,  uint16_t *sector, uint16_t *head) {
            *sector = (lba % disk->sectors) + 1;
            *cylinder = lba / (disk->heads * disk->sectors);
            *head = (lba / disk->sectors) % disk->heads;
        }
        bool Read(DISK *disk, uint32_t lba, uint8_t sectors, void* buffer) {
            uint16_t cylinder, head, sector;

            LBA2CHS(disk, lba, &cylinder, &sector, &head);

            for(int i=0; i < 3; i++) {
                if(ReadSectors(disk->id, cylinder, sector, head, sectors, buffer))
                    return true;
                
                Reset(disk->id);
            }

            return false;
        }
    }
