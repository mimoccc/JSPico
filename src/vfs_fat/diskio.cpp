#include "Device.h"
#include "ff.h"
#include "FileSystem.h"
#include "diskio.h"
#include "Device.h"

// -- C callbacks -------------------------------------------------------------

// disk status from fatfs lib
DSTATUS disk_status(BYTE deviceIndex) {
    DSTATUS stat = STA_NOINIT;
    if (FileSystem::deviceCount() > deviceIndex) {
        Device *device = FileSystem::devices[deviceIndex];
        if (device->ready()) {
            stat = STA_OK;
        }
    }
    return stat;
}

// initialize disk from fatfs lib
DSTATUS disk_initialize(BYTE deviceIndex) {
    DSTATUS stat = STA_NODISK;
    if (FileSystem::deviceCount() > deviceIndex) {
        Device *device = FileSystem::devices[deviceIndex];
        if (device->ready()) {
            stat = STA_OK;
        }
    }
    return stat;
}

// read device from fatfs lib
DRESULT disk_read(BYTE deviceIndex, BYTE *buff, DWORD sector, UINT count) {
    DRESULT res = RES_NOTRDY;
    if (FileSystem::deviceCount() > deviceIndex) {
        Device *device = FileSystem::devices[deviceIndex];
        if (device->ready()) {
            if (sector >= device->totalSize) {
                res = RES_PARERR;
            } else {
                device->read( buff, sector, count);
                res = RES_OK;
            }
        } else {
            res = RES_NOTRDY;
        }
    }
    return res;
}

// write to device from fatfs lib
DRESULT disk_write(BYTE deviceIndex, const BYTE *buff, DWORD sector, UINT count) {
    DRESULT res = RES_NOTRDY;
    if (FileSystem::deviceCount() > deviceIndex) {
        Device *device = FileSystem::devices[deviceIndex];
        if (device->ready()) {
            if (sector >= device->totalSize) {
                res = RES_PARERR;
            } else {
                device->write((BYTE *) buff, sector, count);
                res = RES_OK;
            }
        } else {
            res = RES_NOTRDY;
        }
    }
    return res;
}

// ioct on device from fatfs lib
DRESULT disk_ioctl(BYTE deviceIndex, BYTE cmd, void *buff) {
    DRESULT res = RES_PARERR;
    if (FileSystem::deviceCount() > deviceIndex) {
        Device *device = FileSystem::devices[deviceIndex];
        DWORD b_count = device->blockCount;
        DWORD b_size = device->blockSize;
        switch (cmd) {
            case CTRL_SYNC:
                res = RES_OK;
                break;
            case GET_SECTOR_COUNT:
                *(DWORD *) buff = b_count;
                res = RES_OK;
                break;
            case GET_BLOCK_SIZE:
                *(DWORD *) buff = b_size;
                res = RES_OK;
                break;
            default:
                res = RES_OK;
                break;
        }
    }
    return res;
}

// -- C callbacks -------------------------------------------------------------
