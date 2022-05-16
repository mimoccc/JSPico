#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Device.h"

#include "ffconf.h"
#include "ff.h"
#include "TaskManager.h"
#include "BlinkTaskArgs.h"

const char *TEST_STRING = "Test string";
const char *TEST_F_NAME = "test.txt";
const S_FILE s_files[] = {
        {
                .name = (char *) &TEST_F_NAME,
                .content = (char *) &TEST_STRING,
                .isDirectory = false
        }
};

Device::Device(const char *label) : Device(label, DEFAULT_DISK_SIZE_IN_KB) {}

Device::Device(const char *label, uint32_t sizeInKb) {
    this->devID = nullptr;
    size_t l_size = strlen(label);
    this->label = static_cast<char *>(malloc(12));
    memcpy(this->label, label, (l_size > 12) ? 12 : l_size);
    this->totalSize = sizeInKb * 1024;
    this->blockSize = DEFAULT_DISK_BLOCK_SIZE;
    this->blockCount = this->totalSize / this->blockSize;
    this->data = static_cast<uint8_t *>(malloc(this->totalSize));
    this->mkFatFs((S_FILE **) &s_files, sizeof(s_files));
}

Device::~Device() {
    free(this->label);
    free(this->devID);
    free(this->data);
    this->data = nullptr;
}

void Device::init(unsigned int drvID) {
    char *bf = static_cast<char *>(malloc(64));
    sprintf(bf, "%s%d:", label, drvID);
    size_t s_size = strlen(bf);
    this->devID = static_cast<char *>(malloc(s_size));
    memcpy(this->devID, bf, s_size);
}

//void Device::load() {
//}

//void Device::unload() {
//}

bool Device::ready() {
    return data != nullptr;
}

uint32_t Device::read(uint8_t *buffer, uint32_t sector, uint32_t offset, uint32_t buff_size) {
    if (sector >= blockCount) return -1;
    memcpy(buffer, this->data + ((sector * blockSize) + offset), buff_size);
    return buff_size;
}

uint32_t Device::write(uint8_t *buffer, uint32_t sector, uint32_t offset, uint32_t buff_size) {
    if (sector >= blockCount) return -1;
    memcpy(this->data + ((sector * blockSize) + offset), buffer, buff_size);
    return buff_size;
}

uint32_t Device::read(uint8_t *buffer, uint32_t sector, uint32_t count) {
    return Device::read(buffer, sector * blockSize, 0, (count * blockSize));
}

uint32_t Device::write(uint8_t *buffer, uint32_t sector, uint32_t count) {
    return Device::write(buffer, sector * blockSize, 0, (count * blockSize));
}

uint32_t Device::ioctl(uint8_t devIndex, const uint8_t *scsi_cmd, void *buffer, uint16_t buff_size) {
    void const *response = nullptr;
    uint32_t resp_len = 0;
    bool in_x_fer = true;
    switch (scsi_cmd[0]) {
        case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
            resp_len = 0;
            break;
        case SCSI_CMD_START_STOP_UNIT:
            resp_len = 0;
            break;
        default:
            tud_msc_set_sense(devIndex, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);
            resp_len = -1;
            break;
    }
    if (resp_len > buff_size) resp_len = buff_size;
    if (resp_len > 0) {
        if (in_x_fer) {
            memcpy(buffer, response, resp_len);
        } else {
            // SCSI output
        }
    }
    return resp_len;
}

FRESULT Device::mkFatFs(S_FILE *files[], int file_cnt) {
    FRESULT res = FR_DISK_ERR;
    // try to mount already existing fs
    res = f_mount(&this->fatfs, this->devID, 1);
    // can not be mounted
    if (res != FR_OK) {
        // create new file system
        res = f_mkfs(
                this->devID,
                FM_FAT | FM_FAT32 | FM_SFD,
                this->blockSize,
                buff,
                sizeof(buff)
        );
        // if success/created
        if (res == FR_OK) {
            // mount filesystem
            res = f_mount(&this->fatfs, this->devID, 1);
            if (res == FR_OK) {
                // set label
                res = f_setlabel(this->label);
                if (res != FR_OK) {
                    // signalize error
                    TaskManager::addTaskArgs(new BlinkTaskArgs(nullptr, 100));
                }
            } else {
                // signalize error
                TaskManager::addTaskArgs(new BlinkTaskArgs(nullptr, 100));
            }
        } else {
            // signalize error
            TaskManager::addTaskArgs(new BlinkTaskArgs(nullptr, 100));
        }
    }
    if (file_cnt > 0) {
        publish_files(*files, file_cnt, false);
    }
    return res;
}

void Device::publish_files(S_FILE files[], int file_cnt, bool overwrite) {
    FRESULT res;
    FIL fil;
    FILINFO fno;
    UINT bw;
    for (int i = 0; i < file_cnt; i++) {
        S_FILE file = files[i];
        res = (overwrite) ? FR_NO_FILE : f_stat(file.name, &fno);
        if (res != FR_OK) {
            res = f_open(&fil, file.name, FA_CREATE_NEW | FA_WRITE);
            if (res == FR_OK) {
                f_write(&fil, file.content, sizeof(file.content), &bw);
                f_close(&fil);
            }
        }
    }
}