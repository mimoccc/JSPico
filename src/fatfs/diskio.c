//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "diskio.h"
#include "ff.h"

//-----------------------------------------------------------------------------

extern BYTE *ram_disk;
extern DWORD ram_disk_size;

//-----------------------------------------------------------------------------

DSTATUS disk_initialize(
        BYTE pdrv       /* Physical drive nmuber (0) */
) {
    if (pdrv)
        return STA_NOINIT;
    if (!ram_disk) {
        ram_disk = malloc(ram_disk_size * FF_MIN_SS);
    }
    return ram_disk ? 0 : STA_NOINIT;
}

//-----------------------------------------------------------------------------

DSTATUS disk_status(BYTE pdrv) {
    if (pdrv) return STA_NOINIT;
    return ram_disk ? 0 : STA_NOINIT;
}

//-----------------------------------------------------------------------------

DRESULT disk_read(
        BYTE pdrv,             /* Physical drive nmuber (0) */
        BYTE *buff,            /* Pointer to the data buffer to store read data */
        DWORD sector,          /* Start sector number (LBA) */
        UINT count             /* Number of sectors to read */
) {
    if (pdrv || !ram_disk)
        return RES_NOTRDY;
    if (sector >= ram_disk_size)
        return RES_PARERR;
    memcpy(buff, ram_disk + (sector * FF_MIN_SS), (count * FF_MIN_SS));
    return RES_OK;
}

//-----------------------------------------------------------------------------

DRESULT disk_write(
        BYTE pdrv,            /* Physical drive nmuber (0) */
        const BYTE *buff,     /* Pointer to the data to be written */
        DWORD sector,         /* Start sector number (LBA) */
        UINT count            /* Number of sectors to write */
) {
    if (pdrv || !ram_disk)
        return RES_NOTRDY;
    if (sector >= ram_disk_size)
        return RES_PARERR;
    memcpy(ram_disk + (sector * FF_MIN_SS), buff, (count * FF_MIN_SS));
    return RES_OK;
}

//-----------------------------------------------------------------------------

DRESULT disk_ioctl(
        BYTE pdrv,        /* Physical drive nmuber (0) */
        BYTE ctrl,        /* Control code */
        void *buff        /* Buffer to send/receive data block */
) {
    DRESULT dr;
    dr = RES_ERROR;
    if (!pdrv && ram_disk) {
        switch (ctrl) {
            case CTRL_SYNC:
                dr = RES_OK;
                break;
            case GET_SECTOR_COUNT:
                *(DWORD *) buff = ram_disk_size;
                dr = RES_OK;
                break;
            case GET_BLOCK_SIZE:
                *(DWORD *) buff = 1;
                dr = RES_OK;
                break;
        }
    }
    return dr;
}

//-----------------------------------------------------------------------------