#include <stddef.h>
#include "fat.hpp"
#include "string.hpp"
#include "../drivers/memory.hpp"
#include "ctype.hpp"


#define SECTOR_SIZE             512
#define MAX_PATH_SIZE           256
#define MAX_FILE_HANDLES        10
#define ROOT_DIRECTORY_HANDLE   -1


namespace FAT {
    extern "C" {
        // As defined in boot/FAT12.asm
        typedef struct BootSector{
            uint8_t BootJumpInstruction[3];
            uint8_t oem_name[8];
            uint16_t bytes_per_sector;
            uint8_t sectors_per_cluster;
            uint16_t reserved_sectors;
            uint8_t count;
            uint16_t dir_entry_count;
            uint16_t TotalSectors;
            uint8_t media_descriptor_type;
            uint16_t sectors_per_fat;
            uint16_t SectorsPerTrack;
            uint16_t number_of_heads;
            uint32_t hidden_sectors;
            uint32_t large_sector_count;

            // extended boot record
            uint8_t drive_number;
            uint8_t _reserved;
            uint8_t signature;
            uint32_t volume_id;          // serial number, value doesn't matter
            uint8_t volume_label[11];    // 11 bytes, padded with spaces
            uint8_t system_id[8];

            // ... we don't care about code ...

        } __attribute__((packed)) BootSector;


        typedef struct FILE_Data{
            uint8_t Buffer[SECTOR_SIZE];
            FILE file;
            bool inUse;
            uint32_t first_cluster;
            uint32_t current_cluster;
            uint32_t current_sector;

        } FILE_Data;

        typedef struct
        {
            union
            {
                BootSector BootSector;
                uint8_t BootSectorBytes[SECTOR_SIZE];
            } BS;

            FILE_Data RootDirectory;

            FILE_Data OpenedFiles[MAX_FILE_HANDLES];

        } Data;

        static Data* g_Data;
        static uint8_t* g_Fat = NULL;
        static uint32_t g_DataSectionLba;


        bool ReadBootSector(Disk::DISK* disk)
        {
            return Disk::Read(disk, 0, 1, g_Data->BS.BootSectorBytes);
        }

        bool ReadFat(Disk::DISK* disk)
        {
            return Disk::Read(disk, g_Data->BS.BootSector.reserved_sectors, g_Data->BS.BootSector.sectors_per_fat, g_Fat);
        }

        bool Initialize(Disk::DISK* disk)
        {
            g_Data = (Data*)MEMORY_FAT_ADDR;

            // read boot sector
            if (!ReadBootSector(disk))
            {

                return false;
            }

            // read FAT
            g_Fat = (uint8_t*)g_Data + sizeof(Data);
            uint32_t fatSize = g_Data->BS.BootSector.bytes_per_sector * g_Data->BS.BootSector.sectors_per_fat;
            if (sizeof(Data) + fatSize >= MEMORY_FAT_SIZE)
            {
                return false;
            }

            if (!ReadFat(disk))
            {

                return false;
            }

            // open root directory file
            uint32_t rootDirLba = g_Data->BS.BootSector.reserved_sectors + g_Data->BS.BootSector.sectors_per_fat * g_Data->BS.BootSector.count;
            uint32_t rootDirSize = sizeof(DirectoryEntry) * g_Data->BS.BootSector.dir_entry_count;

            g_Data->RootDirectory.file.Handle = ROOT_DIRECTORY_HANDLE;
            g_Data->RootDirectory.file.IsDirectory = true;
            g_Data->RootDirectory.file.Position = 0;
            g_Data->RootDirectory.file.Size = sizeof(DirectoryEntry) * g_Data->BS.BootSector.dir_entry_count;
            g_Data->RootDirectory.inUse = true;
            g_Data->RootDirectory.first_cluster = rootDirLba;
            g_Data->RootDirectory.current_cluster = rootDirLba;
            g_Data->RootDirectory.current_sector = 0;

            if (!Disk::Read(disk, rootDirLba, 1, g_Data->RootDirectory.Buffer))
            {
                return false;
            }

            // calculate data section
            uint32_t rootDirSectors = (rootDirSize + g_Data->BS.BootSector.bytes_per_sector - 1) / g_Data->BS.BootSector.bytes_per_sector;
            g_DataSectionLba = rootDirLba + rootDirSectors;

            // reset opened files
            for (int i = 0; i < MAX_FILE_HANDLES; i++)
                g_Data->OpenedFiles[i].inUse = false;

            return true;
        }

        uint32_t ClusterToLba(uint32_t cluster)
        {
            return g_DataSectionLba + (cluster - 2) * g_Data->BS.BootSector.sectors_per_cluster;
        }

        FILE* OpenEntry(Disk::DISK* disk, DirectoryEntry* entry)
        {
            // find empty handle
            int handle = -1;
            for (int i = 0; i < MAX_FILE_HANDLES && handle < 0; i++)
            {
                if (!g_Data->OpenedFiles[i].inUse)
                    handle = i;
            }

            // out of handles
            if (handle < 0)
            {

                return NULL;
            }

            // setup vars
            FILE_Data* fd = &g_Data->OpenedFiles[handle];
            fd->file.Handle = handle;
            fd->file.IsDirectory = (entry->Attributes & ATTRIBUTE_DIRECTORY) != 0;
            fd->file.Position = 0;
            fd->file.Size = entry->Size;
            fd->first_cluster = entry->FirstClusterLow + ((uint32_t)entry->FirstClusterHigh << 16);
            fd->current_cluster = fd->first_cluster;
            fd->current_sector = 0;

            if (!Disk::Read(disk, ClusterToLba(fd->current_cluster), 1, fd->Buffer))
            {

                return NULL;
            }

            fd->inUse = true;
            return &fd->file;
        }

        uint32_t NextCluster(uint32_t current_cluster)
        {    
            uint32_t fatIndex = current_cluster * 3 / 2;

            if (current_cluster % 2 == 0)
                return (*(uint16_t*)(g_Fat + fatIndex)) & 0x0FFF;
            else
                return (*(uint16_t*)(g_Fat + fatIndex)) >> 4;
        }

        uint32_t Read(Disk::DISK* disk, FILE* file, uint32_t byteCount, void* dataOut)
        {
            // get file data
            FILE_Data* fd = (file->Handle == ROOT_DIRECTORY_HANDLE) 
                ? &g_Data->RootDirectory 
                : &g_Data->OpenedFiles[file->Handle];

            uint8_t* u8DataOut = (uint8_t*)dataOut;

            // don't read past the end of the file
            if (!fd->file.IsDirectory || (fd->file.IsDirectory && fd->file.Size != 0))
                byteCount = min(byteCount, fd->file.Size - fd->file.Position);

            while (byteCount > 0)
            {
                uint32_t leftInBuffer = SECTOR_SIZE - (fd->file.Position % SECTOR_SIZE);
                uint32_t take = min(byteCount, leftInBuffer);

                memory::memcpy(u8DataOut, fd->Buffer + fd->file.Position % SECTOR_SIZE, take);
                u8DataOut += take;
                fd->file.Position += take;
                byteCount -= take;

                // printf("leftInBuffer=%lu take=%lu\r\n", leftInBuffer, take);
                // See if we need to read more data
                if (leftInBuffer == take)
                {
                    // Special handling for root directory
                    if (fd->file.Handle == ROOT_DIRECTORY_HANDLE)
                    {
                        ++fd->current_cluster;

                        // read next sector
                        if (!Disk::Read(disk, fd->current_cluster, 1, fd->Buffer))
                        {

                            break;
                        }
                    }
                    else
                    {
                        // calculate next cluster & sector to read
                        if (++fd->current_sector >= g_Data->BS.BootSector.sectors_per_cluster)
                        {
                            fd->current_sector = 0;
                            fd->current_cluster = NextCluster(fd->current_cluster);
                        }

                        if (fd->current_cluster >= 0xFF8)
                        {
                            // Mark end of file
                            fd->file.Size = fd->file.Position;
                            break;
                        }

                        // read next sector
                        if (!Disk::Read(disk, ClusterToLba(fd->current_cluster) + fd->current_sector, 1, fd->Buffer))
                        {

                            break;
                        }
                    }
                }
            }

            return u8DataOut - (uint8_t*)dataOut;
        }

        bool ReadEntry(Disk::DISK* disk, FILE* file, DirectoryEntry* dirEntry)
        {
            return FAT::Read(disk, file, sizeof(DirectoryEntry), dirEntry) == sizeof(DirectoryEntry);
        }

        void Close(FILE* file)
        {
            if (file->Handle == ROOT_DIRECTORY_HANDLE)
            {
                file->Position = 0;
                g_Data->RootDirectory.current_cluster = g_Data->RootDirectory.first_cluster;
            }
            else
            {
                g_Data->OpenedFiles[file->Handle].inUse = false;
            }
        }

        bool FindFile(Disk::DISK* disk, FILE* file, const char* name, DirectoryEntry* entryOut)
        {
            char fatName[12];
            DirectoryEntry entry;

            // convert from name to fat name
            memory::memset(fatName, ' ', sizeof(fatName));
            fatName[11] = '\0';

            const char* ext = strchr(name, '.');
            if (ext == NULL)
                ext = name + 11;

            for (int i = 0; i < 8 && name[i] && name + i < ext; i++)
                fatName[i] = toupper(name[i]);

            if (ext != name + 11)
            {
                for (int i = 0; i < 3 && ext[i + 1]; i++)
                    fatName[i + 8] = toupper(ext[i + 1]);
            }

            while (ReadEntry(disk, file, &entry))
            {
                if (memory::memcmp(fatName, entry.Name, 11) == 0)
                {
                    *entryOut = entry;
                    return true;
                }        
            }
            
            return false;
        }

        FILE* Open(Disk::DISK* disk, const char* path)
        {
            char name[MAX_PATH_SIZE];

            // ignore leading slash
            if (path[0] == '/')
                path++;

            FILE* current = &g_Data->RootDirectory.file;

            while (*path) {
                // extract next file name from path
                bool isLast = false;
                const char* delim = strchr(path, '/');
                if (delim != NULL)
                {
                    memory::memcpy(name, path, delim - path);
                    name[delim - path + 1] = '\0';
                    path = delim + 1;
                }
                else
                {
                    unsigned len = strlen(path);
                    memory::memcpy(name, path, len);
                    name[len + 1] = '\0';
                    path += len;
                    isLast = true;
                }
                
                // find directory entry in current directory
                DirectoryEntry entry;
                if (FindFile(disk, current, name, &entry))
                {
                    Close(current);

                    // check if directory
                    if (!isLast && entry.Attributes & ATTRIBUTE_DIRECTORY == 0)
                    {

                        return NULL;
                    }

                    // open new directory entry
                    current = OpenEntry(disk, &entry);
                }
                else
                {
                    Close(current);


                    return NULL;
                }
            }

            return current;
        }
    }
}