#pragma once
#include <stdint.h>
#include "../drivers/disk.hpp"
#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))
    namespace FAT {
extern "C" {

        typedef struct {
            uint8_t Name[11];
            uint8_t Attributes;
            uint8_t _Reserved;
            uint8_t CreatedTimeTenths;
            uint16_t CreatedTime;
            uint16_t CreatedDate;
            uint16_t AccessedDate;
            uint16_t FirstClusterHigh;
            uint16_t ModifiedTime;
            uint16_t ModifiedDate;
            uint16_t FirstClusterLow;
            uint32_t Size;
        } __attribute__((packed)) DirectoryEntry;

        typedef struct {
            int Handle;
            bool IsDirectory;
            uint32_t Position;
            uint32_t Size;
        } FILE;

        enum Attributes{
            ATTRIBUTE_READ_ONLY         = 0x01,
            ATTRIBUTE_HIDDEN            = 0x02,
            ATTRIBUTE_SYSTEM            = 0x04,
            ATTRIBUTE_VOLUME_ID         = 0x08,
            ATTRIBUTE_DIRECTORY         = 0x10,
            ATTRIBUTE_ARCHIVE           = 0x20,
            ATTRIBUTE_LFN               = ATTRIBUTE_READ_ONLY | ATTRIBUTE_HIDDEN | ATTRIBUTE_SYSTEM | ATTRIBUTE_VOLUME_ID
        };

        bool Initialize(Disk::DISK* disk);
        FILE * Open(Disk::DISK* disk, const char* path);
        uint32_t Read(Disk::DISK* disk, FILE* file, uint32_t byteCount, void* dataOut);
        bool ReadEntry(Disk::DISK* disk, FILE* file, DirectoryEntry* dirEntry);
        void Close(FILE* file);
    }
}