#include "fat.hpp"

#define SECTOR_SIZE             512
#define MAX_PATH_SIZE           256
#define MAX_FILE_HANDLES        10
#define ROOT_DIRECTORY_HANDLE   -1

extern "C" {
    // CPP definition of boot/FAT12
    typedef struct __attribute__((packed)) BootSector {
        uint8_t jump[3];
        char oem_name[8];   
        uint16_t bytes_per_sector;
        uint8_t sectors_per_cluster;
        uint16_t reserved_sectors;
        uint8_t fat_count;
        uint16_t dir_entries_count;
        uint16_t total_sectors;
        uint8_t media_descriptor_type;
        uint16_t sectors_per_fat;
        uint16_t sectors_per_track;
        uint16_t number_of_heads;
        uint32_t hidden_sectors;
        uint32_t large_sectors;

        // Extended boot record
        uint8_t drive_number;
        uint8_t _reserved;
        uint8_t bootSignature;
        uint32_t volume_id;
        char volumeLabel[11];
        char fileSystemType[8];
    } BootSector;

    typedef struct __attribute__((packed)) FAT_File_Data{
        char buffer[SECTOR_SIZE];
        FAT_File file;
        bool inUse;
        uint32_t first_cluster;
        uint32_t current_cluster;
        uint32_t current_sector;
    } FAT_File_Data;

    typedef struct FAT_Data{
        union BS{
            BootSector boot;
            char buffer[SECTOR_SIZE];
        } BS;

        FAT_File_Data Root_Dir;

        FAT_File_Data opened_files[MAX_FILE_HANDLES];
    } FAT_Data;

    /* Globally defined params*/
    static FAT_Data *fat_data;
    static uint8_t *FAT = NULL;
    static uint8_t data_section_lba;


    bool ReadBootSector(DISK *disk) {
        return Disk_Read(disk, 0, 1, fat_data->BS.buffer);
    }
    bool Read_FAT(DISK *disk) {
        return Disk_Read(disk, fat_data->BS.boot.reserved_sectors, fat_data->BS.boot.sectors_per_fat, FAT);
    }
    bool FAT_Init(DISK *disk) {
        fat_data = (FAT_Data*)MEMORY_FAT_ADDRESS;
        // Read boot sector
        if(!ReadBootSector(disk))
            return false;
        // Read FAT
        FAT = (uint8_t*)fat_data + sizeof(FAT_Data);
        uint32_t fat_size = fat_data->BS.boot.sectors_per_fat * fat_data->BS.boot.bytes_per_sector;
        if(sizeof(FAT_Data) + fat_size > MEMORY_FAT_SIZE)
            return false;
        if(!Read_FAT(disk))
            return false;
        
        // Read root directory
        uint32_t rootDirLBA = fat_data->BS.boot.reserved_sectors + (fat_data->BS.boot.sectors_per_fat * fat_data->BS.boot.fat_count);
        uint32_t rootDirSize = sizeof(DirectoryEntry) * fat_data->BS.boot.dir_entries_count;

        fat_data->Root_Dir.file.Handle = ROOT_DIRECTORY_HANDLE;
        fat_data->Root_Dir.file.isDirectory = true;
        fat_data->Root_Dir.file.currentCluster = 0;
        fat_data->Root_Dir.file.size = rootDirSize;
        fat_data->Root_Dir.first_cluster = rootDirLBA;
        fat_data->Root_Dir.current_cluster = rootDirLBA;
        fat_data->Root_Dir.current_sector = 0;

        // Read root directory
        if(!Disk_Read(disk, rootDirLBA, 1, fat_data->Root_Dir.buffer))
            return false;
        
        // Calculate data section LBA
        uint32_t rootDirSectors = (rootDirSize + fat_data->BS.boot.bytes_per_sector - 1) / fat_data->BS.boot.bytes_per_sector;
        data_section_lba = rootDirLBA + rootDirSectors;

        // reset opened files
        for(int i = 0; i < MAX_FILE_HANDLES; i++)
            fat_data->opened_files[i].inUse = false;

        return true;
    }
}