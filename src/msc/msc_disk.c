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
#include "diskio.h"
#include "msc_disk.h"

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

static bool mounted = false;
static bool suspended = true;

#define array_size(arr)  (sizeof(arr) / sizeof(arr[0]))

// ramdisc
BYTE *ram_disk;
// ramdisc size
DWORD ram_disk_size;
// fat fs
static FATFS fat_fs;
// buffer
static BYTE buff[4096]; // todo size?
// eject state
static bool ejected = false;
// static files
static S_FILE static_files[1] = {
        {
                .name    = "welcome.txt",
                .content = "Hello, World!\r\n"
        }
};

//PARTITION VolToPart[FF_VOLUMES] = {
//        {0, 1},    /* "0:" ==> ram drive in PD#0 */
//        {1, 0}     /* "1:" ==> flash drive PD#1 */
//};

//-----------------------------------------------------------------------------

bool disk_prepare() {
    FRESULT res;
    FIL fil;
    UINT bw;
    ram_disk_size = DISK_SIZE_KB * 2;
    // create filesystem
//    DWORD plist[] = {100, 0};
//    res = f_fdisk(0, plist, buff);
//    res = f_mkfs("0:", FM_FAT | FM_FAT32 | FM_SFD, DISK_BLOCK_SIZE, buff, sizeof buff);
    res = f_mkfs("", FM_FAT | FM_FAT32 | FM_SFD, DISK_BLOCK_SIZE, buff, sizeof buff);
    if (res == FR_OK) {
        // mount filesystem
        res = f_mount(&fat_fs, "", 0);
        if (res == FR_OK) {
            // set device label
            res = f_setlabel(DEV_USB_VID);
            if (res == FR_OK) {
                // crete device dir
                res = f_mkdir(DEV_DEV_DIR);
                if (res == FR_OK) {
                    // create GPIO folders
                    for (int i = 0; i < 29; i++) {
                        char str_buf[256];
                        sprintf (str_buf, "%s%d", DEV_GPIO_TMP, i);
                        f_mkdir(str_buf);
                        f_chmod(str_buf, AM_RDO | AM_SYS, AM_RDO | AM_SYS);
                    }
                    // create  boot file, for restart
                    f_open(&fil, DEV_BOOT_FILE, FA_CREATE_NEW | FA_WRITE);
                    f_close(&fil);
                }
                // copy files - todo
                if (res == FR_OK) {
                    // create all files need
                    for (int i = 0; i < array_size(static_files); i++) {
                        S_FILE file = static_files[i];
                        res = f_open(&fil, file.name, FA_CREATE_NEW | FA_WRITE);
                        if (res == FR_OK) {
                            f_write(&fil, file.content, strlen(file.content), &bw);
                            f_close(&fil);
                        }
                    }
                }
            }
            // unmount
            f_mount(0, "", 0);
        }
    }
    return res == 0;
}

//-----------------------------------------------------------------------------

void tud_mount_cb(void) {
    mounted = true;
}

//-----------------------------------------------------------------------------

void tud_umount_cb(void) {
    mounted = false;
}

//-----------------------------------------------------------------------------

void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
    suspended = true;
}

//-----------------------------------------------------------------------------

void tud_resume_cb(void) {
    suspended = false;
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
    *block_count = DISK_BLOCK_NUM; // todo from fatfs
    *block_size = DISK_BLOCK_SIZE; // sizeof(fat_fs.win);
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
    board_led_write(0);
    if (lba >= DISK_BLOCK_NUM) {
        return -1;
    }
    memcpy(buffer, ram_disk + (DISK_BLOCK_SIZE * lba) + offset, bufsize);
    board_led_write(1);
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
    board_led_write(0);
    if (lba >= DISK_BLOCK_NUM) {
        return -1;
    }
    memcpy(ram_disk + (DISK_BLOCK_SIZE * lba) + offset, buffer, bufsize);
    board_led_write(1);
    return (int32_t) bufsize;
}

//-----------------------------------------------------------------------------

int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void *buffer, uint16_t bufsize) {
    void const *response = NULL;
    int32_t resp_len = 0;
    bool in_xfer = true;
    switch (scsi_cmd[0]) { // todo removal
        case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
            // Host is about to read/write etc ... better not to disconnect disk
            resp_len = 0;
            break;
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