#include "fat.hpp"
#include "string.hpp"
#include "ctype.hpp"

#define SECTOR_SIZE             512
#define MAX_PATH_SIZE           256
#define MAX_FILE_HANDLES        10
#define ROOT_DIRECTORY_HANDLE   -1
void * __gxx_personality_v0=0;
void * _Unwind_Resume =0;
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
    uint32_t FAT_cluster_to_lba(uint32_t cluster) {
        return data_section_lba + ((cluster - 2) * fat_data->BS.boot.sectors_per_cluster);
    }
    uint32_t FAT_NextCluster(uint32_t currentCluster) {
        uint32_t index = currentCluster * 3 / 2;
        if(currentCluster % 2 == 0) {
            return (*(uint16_t*)FAT + index) & 0xFFF;
        } else {
            return (*(uint16_t*)FAT + index) >> 4;
        }
    }
    uint32_t FAT_Read(DISK *disk, FAT_File *file, uint32_t count, void* buffer) {
        
        FAT_File_Data *file_data = (file->Handle == ROOT_DIRECTORY_HANDLE) ? &fat_data->Root_Dir : &fat_data->opened_files[file->Handle];
        

        uint8_t *buffer8 = (uint8_t*)buffer;

        if(!file_data->file.isDirectory || (file_data->file.isDirectory && file_data->file.size !=0))
            count = min(count, file_data->file.size - file_data->file.currentCluster);
        
        while(count > 0) {
            uint32_t leftBuffer = SECTOR_SIZE - (file->currentCluster % SECTOR_SIZE);
            uint32_t take = min(count, leftBuffer);

            memory::memcpy(buffer8, file_data->buffer + (file->currentCluster % SECTOR_SIZE), take);
            buffer8 += take;
            file_data->file.currentCluster += take;
            count -= take;

            if(leftBuffer == take) {
                if(file_data->file.Handle == ROOT_DIRECTORY_HANDLE) {
                    ++file_data->current_cluster;
                    if(!Disk_Read(disk, file_data->current_cluster, 1, file_data->buffer))
                        break;
                }
                else {
                    if(++file_data->current_sector >= fat_data->BS.boot.sectors_per_cluster) {
                        file_data->current_sector = 0;
                        file_data->current_cluster = FAT_NextCluster(file_data->current_cluster);
                    }
                    if(file_data->current_cluster >= 0xFF8) {
                        file_data->file.size = file_data->file.currentCluster;
                        break;
                    }
                    if(!Disk_Read(disk, FAT_cluster_to_lba(file_data->current_cluster) + file_data->current_sector, 1, file_data->buffer)) {
                        break;
                    }
                }
            }
        }
        return buffer8 - (uint8_t*)buffer;
    }
    bool FAT_ReadEntry(DISK *disk, FAT_File *file, DirectoryEntry *entry) {
        return FAT_Read(disk, file, sizeof(DirectoryEntry), entry) == sizeof(DirectoryEntry);
    }
    void FAT_Close(FAT_File *file) {
        if(file->Handle == ROOT_DIRECTORY_HANDLE) {
            file->currentCluster = 0;
            fat_data->Root_Dir.current_cluster = fat_data->Root_Dir.first_cluster;
        } else {
            fat_data->opened_files[file->Handle].inUse = false;
        }
    }
    bool FAT_FindFile(DISK *disk, FAT_File *file, const char* name, DirectoryEntry *out_entry) {
        char lfn[12];
        DirectoryEntry entry;

        memory::memset(lfn, ' ', sizeof(lfn));
        lfn[11] = '\0';

        const char *ext = strchr(name, ',');
        if(ext == NULL)
            ext = name + 11;
        
        for(int i = 0; i < 8 && name[i] && name + i < ext; i++)
            lfn[i] = toupper(name[i]);
        if(ext != name + 11) {
            for(int i = 0; i < 3 && ext[i+1]; i++)
                lfn[i+8] = toupper(ext[i+1]);
        }
        while(FAT_ReadEntry(disk, file, &entry)) {
            if(memory::memcmp(lfn, entry.name, 11) == 0) {
                *out_entry = entry;
                return true;
            }
        }
        return false;
    }
}
FAT_File* FAT_OpenEntry(DISK* disk, DirectoryEntry* entry) {
    // find empty handle
    int handle = -1;
    for (int i = 0; i < MAX_FILE_HANDLES && handle < 0; i++)
    {
        if (!fat_data->opened_files[i].inUse)
            handle = i;
    }

    // out of handles
    if (handle < 0)
    {
        return NULL;
    }

    // // setup vars
    //FAT_File_Data* fd = &fat_data->opened_files[handle];
    fat_data->opened_files[handle].file.Handle = handle;
    fat_data->opened_files[handle].file.isDirectory = (entry->attributes & FAT_ATTR_DIRECTORY) != 0;
    fat_data->opened_files[handle].file.currentCluster = 0;
    fat_data->opened_files[handle].file.size = entry->size;
    fat_data->opened_files[handle].first_cluster = entry->firstCluster + ((uint32_t)entry->firstClusterHigh << 16);
    fat_data->opened_files[handle].current_cluster = fat_data->opened_files[handle].first_cluster;
    fat_data->opened_files[handle].current_sector = 0;

    uint32_t lba = FAT_cluster_to_lba(fat_data->opened_files[handle].current_cluster);

    if (!Disk_Read(disk, lba, 1,fat_data->opened_files[handle].buffer))
        return NULL;


    // fd->inUse = true;
    // return &fd->file;
    return NULL;
}
FAT_File* FAT_Open(DISK* disk, const char* path) {
    char name[MAX_PATH_SIZE];

    if(path[0] == '/') {
        path++;
    }
    FAT_File *current = &fat_data->Root_Dir.file;
    while(*path) {
        bool isLast = false;
        const char *delim = strchr(path, '/');
        if(delim != NULL) {
            memory::memcpy(name, path, delim - path);
            name[delim - path + 1] = '\0';
            path = delim + 1;
        } else {
            unsigned len = strlen(path);
            memory::memcpy(name, path, len);
            name[len + 1] = '\0';
            path += len;
            isLast = true;
        }
        DirectoryEntry entry;
        if(FAT_FindFile(disk, current, name, &entry)) {
            FAT_Close(current);
            if(!isLast && (entry.attributes & FAT_ATTR_DIRECTORY == 0)) {
                return NULL;
            }
            current = FAT_OpenEntry(disk, &entry);
        } else {
            FAT_Close(current);
            return NULL;
        }
    }
    return current;
}

