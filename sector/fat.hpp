#pragma once
#include <stdint.h>
#include <cstddef>
#include "../drivers/disk.hpp"
#include "../drivers/memory.hpp"

// IMPLEMENTATION OF THE FAT FILESYSTEM
extern "C" {
    // Description of an entry in the FAT filesystem
    typedef struct __attribute__((packed)) DirectoryEntry {
        char name[11];
        uint8_t attributes;
        uint8_t _reserved;
        uint8_t CreatetimeTenths;
        uint16_t createTime;
        uint16_t createDate;
        uint16_t lastAccessDate;
        uint16_t firstClusterHigh;
        uint16_t lastWriteTime;
        uint16_t lastModifiedDate;
        uint16_t firstCluster;
        uint32_t size;
    } DirectoryEntry;
    // Description of a file in the FAT filesystem
    typedef struct __attribute__((packed)) FAT_File {
        int Handle;
        bool isDirectory;
        uint32_t size;
        uint32_t currentCluster;   

    } FAT_File;
    // Attributes of a file in the FAT filesystem
    enum FAT_ATTRIBUTES {
        FAT_ATTR_READ_ONLY = 0x01,
        FAT_ATTR_HIDDEN = 0x02,
        FAT_ATTR_SYSTEM = 0x04,
        FAT_ATTR_VOLUME_ID = 0x08,
        FAT_ATTR_DIRECTORY = 0x10,
        FAT_ATTR_ARCHIVE = 0x20,
        FAT_ATTR_LONG_NAME = FAT_ATTR_READ_ONLY | FAT_ATTR_HIDDEN | FAT_ATTR_SYSTEM | FAT_ATTR_VOLUME_ID
    };

    bool FAT_Init(DISK *disk);
    FAT_File FAT_Open(DISK *disk, const char *path);
    uint32_t FAT_Read(DISK *disk, FAT_File *file, uint32_t size, void *buffer);
    bool FAT_ReadEntry(DISK *disk, FAT_File *file, DirectoryEntry *entry);
    void FAT_Close(FAT_File *file);
}