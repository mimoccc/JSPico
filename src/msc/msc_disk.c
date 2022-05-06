//-----------------------------------------------------------------------------
// Mass storage disc
//-----------------------------------------------------------------------------

//#include "bsp/board.h"
#include "tusb.h"
//#include "pico/stdlib.h"
//#include "hardware/flash.h"
//#include "flash/flash_disk.h"
#include "board.h"
#include <stdlib.h>
//#include <stdint.h>
#include <stdbool.h>
//#include <stdio.h>
#include <string.h>
#include <time.h>
#include "fatfs/ff.h"
#include "diskio.h"
#include "msc_disk.h"
#include "utils/base64.h"
#include "main.h"

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

extern bool mounted;
extern bool suspended;
extern BYTE *ram_disk;
extern DWORD ram_disk_size;
extern FATFS fat_fs;
extern bool ejected;
extern unsigned char buff[DISK_BLOCK_SIZE];

//-----------------------------------------------------------------------------
// todo type & flash
bool disk_prepare(DISK_TYPE type, S_FILE files[], int file_cnt) {
    FRESULT res;
    FIL fil;
    FILINFO fno;
    UINT bw;
    // try to mount already existing fs
    res = f_mount(&fat_fs, "", 1);
    // can not be mounted
    if (res != FR_OK) {
        // create new file system
        res = f_mkfs("", FM_FAT | FM_FAT32 | FM_SFD, DISK_BLOCK_SIZE, buff, sizeof (buff));
        // if success/created
        if (res == FR_OK) {
            // mount filesystem
            res = f_mount(&fat_fs, "", 1);
        }
    }
    // if one ok, mount or create
    if (res == FR_OK) {
        // set device label
        res = f_setlabel(DEV_USB_VID);
//            if (res == FR_OK) {
        // crete device dir
//                res = f_mkdir(DEV_DEV_DIR);
//                if (res == FR_OK) {
//                    // create GPIO folders
//                    for (int i = 0; i < 29; i++) {
//                        char str_buf[256];
//                        sprintf (str_buf, "%s%d", DEV_GPIO_TMP, i);
//                        f_mkdir(str_buf);
//                        f_chmod(str_buf, AM_RDO | AM_SYS, AM_RDO | AM_SYS);
//                    }
//                    // create  boot file, for restart
//                    f_open(&fil, DEV_BOOT_FILE, FA_CREATE_NEW | FA_WRITE);
//                    f_close(&fil);
//                }
        // check or copy files - todo
        if (res == FR_OK) {
            // create all files if need
            for (int i = 0; i < file_cnt; i++) {
                S_FILE file = files[i];
                res = f_stat(file.name, &fno);
                if (res != FR_OK) {
                    res = f_open(&fil, file.name, FA_CREATE_NEW | FA_WRITE);
                    if (res == FR_OK) {
                        size_t decode_size = strlen(file.content);
                        unsigned char * decoded_data = base64_decode(
                                file.content,
                                decode_size,
                                &decode_size
                                );
                        f_write(&fil, decoded_data, decode_size, &bw);
                        f_close(&fil);
                        free(decoded_data);
                    }
                }
            }
        }
    }
    // return result
    return res == FR_OK;
}

//-----------------------------------------------------------------------------

void tud_mount_cb(void) {
    mounted = true;
}

void tud_umount_cb(void) {
    mounted = false;
    // umount disc
    // f_mount(0, "", 0);
}

void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
    suspended = true;
}

void tud_resume_cb(void) {
    suspended = false;
}

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

bool tud_msc_test_unit_ready_cb(uint8_t lun) {
    (void) lun;
    if (ejected) {
        tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3a, 0x00);
    }
    return !ejected;
}

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

bool tud_msc_is_writable_cb(uint8_t lun) {
    (void) lun;
    return true;
}

// lun device num
// lba start cluster
// offset offset in cluster
// buffer byte buffer
// bufsize byte buffer size
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize) {
    if (lba >= DISK_BLOCK_NUM) {
        return -1;
    }
    memcpy(buffer, ram_disk + (DISK_BLOCK_SIZE * lba) + offset, bufsize);
//    disk_read(0, buffer, lba, bufsize / DISK_BLOCK_SIZE); // todo offset
    return (int32_t) bufsize;
}

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t buffsize) {
    if (lba >= DISK_BLOCK_NUM) {
        return -1;
    }
    memcpy(ram_disk + (DISK_BLOCK_SIZE * lba) + offset, buffer, buffsize);
//    disk_write(0, buffer, lba, buffsize); // todo offsett
    return (int32_t) buffsize;
}

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

bool msc_disk_init(DISK_TYPE type, S_FILE files[], int file_cnt) {
    return disk_prepare(type, files, file_cnt);
}

void msc_disk_task() {
    // no op yet
}

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------