#ifndef JS_PICO_VFS_FAT_H
#define JS_PICO_VFS_FAT_H

#include <cstdlib>
#include <cstring>
#include <pico/bootrom.h>
#include <pico/stdlib.h>
#include "diskio.h"
#include "ff.h"
#include "base64.h"

// file structure for static rom files
typedef struct {
    char *name;
    char *content;
    bool isDirectory;
} S_FILE;

// basic sector size / block size
#define DEFAULT_DISK_BLOCK_SIZE 512

/* Definitions of physical drive number for each drive */
#define DEV_RAM     0   /* map ramdisk to physical drive 0 */
#define DEV_MMC     1   /* map MMC/SD card to physical drive 1 */
#define DEV_USB     2   /* map USB MSD to physical drive 2 */
#define DEV_FLASH   3   /* map Flash to physical drive 3 */

class VfsFat {
private:
    FATFS *fat_fs = {};
    void *buff;
    void *disc_data;
public:
    char *device_label;
    uint total_size;
    uint blocks;
    uint block_size;
    bool mounted;

    VfsFat(char *device_label, uint size_in_kb) : VfsFat(device_label, (size_in_kb / 2), 512) {}

    VfsFat(char *device_label, uint blocks, uint block_size) {
        FRESULT res;
        this->device_label = device_label;
        this->block_size = block_size;
        this->blocks = blocks;
        this->total_size = blocks * block_size;
        this->mounted = false;
        this->buff = malloc(block_size);
        this->disc_data = malloc(blocks * block_size);
        // try to mount already existing fs
        res = f_mount(fat_fs, "", 1);
        // can not be mounted
        if (res != FR_OK) {
            // create new file system
            res = f_mkfs(
                    "",
                    FM_FAT | FM_FAT32 | FM_SFD,
                    this->block_size,
                    this->buff,
                    this->total_size
            );
            // if success/created
            if (res == FR_OK) {
                // mount filesystem
                res = f_mount(fat_fs, "", 1);
                mounted = true;
                if (res == FR_OK) {
                    // set device label
                    f_setlabel(device_label);
                }
            }
        }
    }

    ~VfsFat() {
        this->device_label = NULL;
        this->block_size = 0;
        this->blocks = 0;
        this->total_size = 0;
        this->mounted = false;
        free(this->disc_data);
        free(this->fat_fs);
        free(this->buff);
    }

    void publish_files(S_FILE files[], int file_cnt) {
        FRESULT res;
        FIL fil;
        FILINFO fno;
        UINT bw;
        if (mounted) {
            for (int i = 0; i < file_cnt; i++) {
                S_FILE file = files[i];
                res = f_stat(file.name, &fno);
                if (res != FR_OK) {
                    res = f_open(&fil, file.name, FA_CREATE_NEW | FA_WRITE);
                    if (res == FR_OK) {
                        size_t decode_size = strlen(file.content);
                        unsigned char *decoded_data = base64_decode(
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

//    void createDir(char* dir_name) {
//        FRESULT res;
//        FIL fil;
//        FILINFO fno;
//        UINT bw;
//        if (mounted) {
//         //create device dir
//                res = f_mkdir(dir_name);
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
//        }
//    }
};

// FATFS

DSTATUS disk_status(BYTE pdrv) {
    DSTATUS stat = STA_NODISK;
    int result;
    switch (pdrv) {
        case DEV_RAM :
//            result = RAM_disk_status();
            // translate the reslut code here
            return stat;
        case DEV_MMC :
//            result = MMC_disk_status();
            // translate the reslut code here
            return stat;
        case DEV_USB :
//            result = USB_disk_status();
            // translate the reslut code here
            return stat;
        case DEV_FLASH:
            //            result = USB_disk_status();
            // translate the reslut code here
            return stat;
    }
    return STA_NOINIT;
}

DSTATUS disk_initialize(BYTE pdrv) {
    DSTATUS stat = STA_NODISK;
    int result;
    switch (pdrv) {
        case DEV_RAM :
//            result = RAM_disk_initialize();
            // translate the reslut code here
            return stat;
        case DEV_MMC :
//            result = MMC_disk_initialize();
            // translate the reslut code here
            return stat;
        case DEV_USB :
//            result = USB_disk_initialize();
            // translate the reslut code here
            return stat;
        case DEV_FLASH :
//            result = USB_disk_initialize();
            // translate the reslut code here
            return stat;
    }
    return STA_NOINIT;
}

DRESULT disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count) {
    DRESULT res = RES_NOTRDY;
    int result;
    switch (pdrv) {
        case DEV_RAM :
            // translate the arguments here
//            result = RAM_disk_read(buff, sector, count);
            // translate the reslut code here
            return res;
        case DEV_MMC :
            // translate the arguments here
//            result = MMC_disk_read(buff, sector, count);
            // translate the reslut code here
            return res;
        case DEV_USB :
            // translate the arguments here
//            result = USB_disk_read(buff, sector, count);
            // translate the reslut code here
            return res;
        case DEV_FLASH :
            // translate the arguments here
//            result = USB_disk_read(buff, sector, count);
            // translate the reslut code here
            return res;
    }
    return RES_PARERR;
}

DRESULT disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count) {
    DRESULT res = RES_NOTRDY;
    int result;
    switch (pdrv) {
        case DEV_RAM :
            // translate the arguments here
//            result = RAM_disk_write(buff, sector, count);
            // translate the reslut code here
            return res;
        case DEV_MMC :
            // translate the arguments here
//            result = MMC_disk_write(buff, sector, count);
            // translate the reslut code here
            return res;
        case DEV_USB :
            // translate the arguments here
//            result = USB_disk_write(buff, sector, count);
            // translate the reslut code here
            return res;
        case DEV_FLASH :
            // translate the arguments here
//            result = USB_disk_write(buff, sector, count);
            // translate the reslut code here
            return res;
    }
    return RES_PARERR;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
    DRESULT res = RES_NOTRDY;
    int result;
    switch (pdrv) {
        case DEV_RAM :
            // Process of the command for the RAM drive
            return res;
        case DEV_MMC :
            // Process of the command for the MMC/SD card
            return res;
        case DEV_USB :
            // Process of the command the USB drive
            return res;
        case DEV_FLASH :
            // Process of the command the USB drive
            return res;
    }
    return RES_PARERR;
}

#endif //JS_PICO_VFS_FAT_H