//------------------------------------------------------------------------------
// Virtual disk implementation
//------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "board.h"
//------------------------------------------------------------------------------
#define STRING_BYTE_SIZE(a) (sizeof(a) - 1)
#define packed __attribute__((__packed__))
//------------------------------------------------------------------------------
// virtual disk size in KB
#define DISK_SIZE (32 * 1024)
// virtual disk block size, default fat 512
#define DISK_BLOCK_SIZE 512
// virtual disc sectors
#define DISK_BLOCK_NUM (DISK_SIZE / DISK_BLOCK_SIZE)
// virtual disk volume label
#define DISK_NAME "PicoJS    "
//------------------------------------------------------------------------------
// bootstrap jump
#define D_BIOS_JUMP_CODE { 0xeb, 0x3c, 0x90 }
// just label of creator , can be anything
#define D_BIOS_CREATOR "MJDev   "
// bytes per sector 512, 1024, 2048, 4096
#define D_BIOS_BPS DISK_BLOCK_SIZE
// sectors per cluster 1, 2, 4, 8, 16, 32, 64, 128
#define D_BIOS_SPC 1
// reserved sectors (fat12=1, Fat16 = 1, FAT32=32)
#define D_BIOS_RS 1
// fat copies, 1
#define D_BIOS_FAT_CNT 1
// root dir entry default size
#define D_BIOS_RDE_ITEM_SIZE 32
// root dir entries (224 default, fat32-0, fat16-512)
#define D_BIOS_MAX_ROOT_DE 16
// root dir komplete size
#define D_BIOS_RDE_SIZE (D_BIOS_MAX_ROOT_DE*D_BIOS_RDE_ITEM_SIZE)
// total number of sectors in the filesystem 2880, in case the partition is not
// FAT32 and smaller than 32 MB.
#define D_BIOS_TOTAL_SECTORS (DISK_SIZE/DISK_BLOCK_SIZE)
// media descriptor type (f0: 1.4 MB floppy, f8: hard disk; see below)
#define D_BIOS_RES_1 0xf8
// number of sectors per FAT (9), 0 for FAT32.
#define D_BIOS_SECS_PER_FAT 1
// number of sectors per track (12)
#define D_BIOS_SECS_PER_TRACK 16
// number of heads (2, for a double-sided diskette)
#define D_BIOS_HEADS 1
// number of hidden sectors (0)
#define D_BIOS_RES_2 0
// total sectors per fat
#define D_BIOS_TOT_SECS_PER_FAT 0
// Logical Drive Number 0x0080 default, bts
#define D_BIOS_RES_3 0x0080
// boot signature
#define D_BIOS_BOOT_SIGN 0x29
// volume id
#define D_BIOS_VOLUME_ID { 0x3C, 0xC3, 0x0d , 0x8c }
// volume label, max 11 chars
#define D_BIOS_VOLUME_LABEL DISK_NAME
// file system type, should be FAT12, FAT16 or FAT32, ...
#define D_BIOS_FS_TYPE "FAT12   "
// boot sector end signature
#define D_BOOT_SECTOR_SIGN 0xaa55
// test file
#define README0_CONTENTS "LUN0: This is tinyusb's MassStorage Class demo.\r\n\r\n\
If you find any bugs or get any questions, feel free to file an\r\n\
issue at github.com/hathach/tinyusb"
// test file size
#define README0_CONTENT_SIZE STRING_BYTE_SIZE(README0_CONTENTS)
//------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct {
    uint8_t name[8];      // file name , if first byte is: 0x00 - final entry, 0xe5 - deleted
    uint8_t ext[3];       // file extension
    uint8_t attrs;        // attrs, 0x01: read only, 0x02: hidden, 0x04: system, 0x08: volume label, 0x10: directory, 0x20: archive
    uint8_t reserved;     // reserved
    uint8_t created[5];   // time created fat style
    int16_t last_access;  // last accessed
    int16_t cluster_high; // for fat32
    int32_t written;      // written date/time
    int16_t cluster_low;  // low-order bytes of the first cluster address
    int32_t size;         // file size
} DIR_ENTRY;
#pragma pack(pop)
//------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct {
    uint8_t BIOS_BOOT_JUMP_CODE[3];
    uint8_t BIOS_CREATOR[8];
    uint16_t BIOS_BYTES_PER_SECTOR;
    uint8_t BIOS_SECTOR_PER_CLUSTER;
    uint16_t BIOS_RESERVED;
    uint8_t BIOS_FAT_COUNT;
    uint16_t BIOS_MAX_ROOT_DIR_ENTRIES;
    uint16_t BIOS_TOTAL_SECTORS;
    uint8_t BIOS_RESERVED_1;
    uint16_t BIOS_SECS_PER_FAT;
    uint16_t BIOS_SECS_PER_TRACK;
    uint16_t BIOS_HEADS;
    uint32_t BIOS_RESERVED_2;
    uint32_t BIOS_TOTAL_SECS_PER_FAT;
    uint16_t BIOS_RESERVED_3;
    uint8_t BIOS_BOOT_SIGN;
    uint32_t BIOS_VOLUME_ID;
    uint8_t BIOS_VOLUME_LABEL[11];
    uint8_t BIOS_FS_TYPE[8];
    uint8_t BOOT_CODE[448];
    uint16_t BOOT_SECTOR_SIGN;
    uint8_t FAT[DISK_BLOCK_SIZE];
    uint8_t ROOT_DIR[D_BIOS_RDE_SIZE];
    uint8_t DATA[DISK_BLOCK_SIZE];
} DEVICE;
#pragma pack(pop)
//------------------------------------------------------------------------------
const DEVICE device_data = {
        .BIOS_BOOT_JUMP_CODE = D_BIOS_JUMP_CODE,
        .BIOS_CREATOR = D_BIOS_CREATOR,
        .BIOS_BYTES_PER_SECTOR = D_BIOS_BPS,
        .BIOS_SECTOR_PER_CLUSTER =D_BIOS_SPC,
        .BIOS_RESERVED = D_BIOS_RS,
        .BIOS_FAT_COUNT = D_BIOS_FAT_CNT,
        .BIOS_MAX_ROOT_DIR_ENTRIES = D_BIOS_MAX_ROOT_DE,
        .BIOS_TOTAL_SECTORS =D_BIOS_TOTAL_SECTORS,
        .BIOS_RESERVED_1=D_BIOS_RES_1,
        .BIOS_SECS_PER_FAT =D_BIOS_SECS_PER_FAT,
        .BIOS_SECS_PER_TRACK = D_BIOS_SECS_PER_TRACK,
        .BIOS_HEADS=D_BIOS_HEADS,
        .BIOS_RESERVED_2 =D_BIOS_RES_2,
        .BIOS_TOTAL_SECS_PER_FAT =D_BIOS_TOT_SECS_PER_FAT,
        .BIOS_RESERVED_3 = D_BIOS_RES_3,
        .BIOS_BOOT_SIGN=D_BIOS_BOOT_SIGN,
        .BIOS_VOLUME_ID = D_BIOS_VOLUME_ID,
        .BIOS_VOLUME_LABEL = D_BIOS_VOLUME_LABEL,
        .BIOS_FS_TYPE = D_BIOS_FS_TYPE,
        .BOOT_CODE = {},
        .BOOT_SECTOR_SIGN = D_BOOT_SECTOR_SIGN,
        .FAT = {
                0xF8, 0xFF, 0xFF, 0xFF, 0x0F
                // // first 2 entries must be F8FF, third entry is cluster end of readme file
        },
        .ROOT_DIR = {
                // first entry is volume label
                'J', 'S', 'P', 'i', 'c', 'o', ' ', ' ',
                ' ', ' ', ' ', 0x08, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x6D,
                0x65, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                // second entry is readme file
                'R', 'E', 'A', 'D', 'M', 'E', '0', ' ',
                'T', 'X', 'T', 0x20, 0x00, 0xC6, 0x52, 0x6D,
                0x65, 0x43, 0x65, 0x43, 0x00, 0x00, 0x88, 0x6D,
                0x65, 0x43, 0x02, 0x00, README0_CONTENT_SIZE, 0x00, 0x00, 0x00
                // readme's files size (4 Bytes)
        },
        .DATA = README0_CONTENTS
};
const uint8_t *device_data_ptr = (uint8_t *) &device_data;
//------------------------------------------------------------------------------