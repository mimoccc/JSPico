//-----------------------------------------------------------------------------
// Mass storage disc
//-----------------------------------------------------------------------------

#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "flash/flash.h"
#include "board.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "fatfs/ff.h"
#include "fatfs/diskio.h"

//-----------------------------------------------------------------------------
#if CFG_TUD_MSC
//-----------------------------------------------------------------------------

#if FF_MIN_SS != FF_MAX_SS
#error Sector size must be fixed at any value
#endif

#define MIN_FAT16         4086U        /* Minimum number of clusters for FAT16 */
#define MIN_FAT32        65526U        /* Minimum number of clusters for FAT32 */

#define BPB_NumFATs          16        /* Number of FAT copies (1) */
#define BPB_RootEntCnt       17        /* Number of root directory entries for FAT12/16 (2) */
#define BPB_TotSec16         19        /* Volume size [sector] (2) */
#define BPB_FATSz16          22        /* FAT size [sector] (2) */
#define BPB_TotSec32         32        /* Volume size [sector] (4) */
#define BPB_FATSz32          36        /* FAT size [sector] (4) */

extern DWORD get_fat(FFOBJID *obj, DWORD);        /* Read an FAT item */
extern DWORD ld_dword(const BYTE *ptr);            /* Load a 4-byte little-endian word */
extern WORD ld_word(const BYTE *ptr);            /* Load a 2-byte little-endian word */
extern void st_word(BYTE *ptr, WORD val);        /* Store a 2-byte word in little-endian */
extern void st_dword(BYTE *ptr, DWORD val);        /* Store a 4-byte word in little-endian */

BYTE *ram_disk;        /* Poiter to the RAM disk */
DWORD ram_disk_size;    /* Size of RAM disk in unit of sector */

static FATFS fat_fs;
static BYTE buff[4096];

// eject state
static bool ejected = false;

//-----------------------------------------------------------------------------

bool disk_prepare() {
    FRESULT res;
    FIL fil;
    UINT bw;
    UINT csz = DISK_BLOCK_SIZE;
    ram_disk_size = DISK_SIZE_KB * 2;
    res = f_mkfs("", FM_FAT | FM_FAT32 | FM_SFD, csz, buff, sizeof buff);
    if (res == FR_OK) {
        res = f_mount(&fat_fs, "", 0);
        if (res == FR_OK) {
            res = f_setlabel(DEV_USB_VID);
            if (res == FR_OK) {
                res = f_open(&fil, "welcome.txt", FA_CREATE_NEW | FA_WRITE);
                if (res == FR_OK) {
                    f_write(&fil, "Hello, World!\r\n", 15, &bw);
                    f_close(&fil);
                }
            }
            f_mount(0, "", 0);
        }
    }
    return res == 0;
}

//-----------------------------------------------------------------------------

void tud_msc_inquiry_cb(
        uint8_t lun,
        uint8_t vendor_id[8],
        uint8_t product_id[16],
        uint8_t product_rev[4]
) {
    (void) lun;
    memcpy(vendor_id, DEV_USB_VID, strlen(DEV_USB_VID));
    memcpy(product_id, DEV_USB_PID, strlen(DEV_USB_PID));
    memcpy(product_rev, DEV_USB_REV, strlen(DEV_USB_REV));
}

//-----------------------------------------------------------------------------

bool tud_msc_test_unit_ready_cb(uint8_t lun) {
    (void) lun;
    if (ejected) {
        tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3a, 0x00);
    }
    return !ejected;
}

//-----------------------------------------------------------------------------

void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size) {
    (void) lun;
    *block_count = DISK_BLOCK_NUM;
    *block_size = DISK_BLOCK_SIZE;
}

//-----------------------------------------------------------------------------

bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject) {
    (void) lun;
    (void) power_condition;
    if (load_eject) {
        if (start) {
            // load disk storage
            ejected = false;
        } else {
            // unload disk storage
            ejected = true;
        }
    }
    return true;
}

//-----------------------------------------------------------------------------

int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize) {
    (void) lun;
    if (lba >= DISK_BLOCK_NUM) {
        return -1;
    }
    memcpy(buffer, ram_disk + (DISK_BLOCK_SIZE * lba) + offset, bufsize);
    return (int32_t) bufsize;
}

//-----------------------------------------------------------------------------

bool tud_msc_is_writable_cb(uint8_t lun) {
    (void) lun;
    return true;
}

//-----------------------------------------------------------------------------

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize) {
    (void) lun;
    if (lba >= DISK_BLOCK_NUM) {
        return -1;
    }
    memcpy(ram_disk + (DISK_BLOCK_SIZE * lba) + offset, buffer, bufsize);
    return (int32_t) bufsize;
}

//-----------------------------------------------------------------------------

int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void *buffer, uint16_t bufsize) {
    void const *response = NULL;
    int32_t resp_len = 0;
    bool in_xfer = true;
    switch (scsi_cmd[0]) {
        default:
            tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);
            resp_len = -1;
            break;
    }
    if (resp_len > bufsize) resp_len = bufsize;
    if (resp_len > 0) {
        if (in_xfer) {
            memcpy(buffer, response, (size_t) resp_len);
        } else {
            // SCSI output
        }
    }
    return (int32_t) resp_len;
}

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------