#ifndef JS_PICO_DEVICE_H
#define JS_PICO_DEVICE_H

#include "pico/stdlib.h"
#include "pico/bootrom.h"

//#include <cassert>
#include <cinttypes>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "tusb.h"
#include "ffconf.h"
#include "diskio.h"
#include "ff.h"

#define DEFAULT_DISK_BLOCK_SIZE 512
#define DEFAULT_DISK_SIZE_IN_KB 64

// file structure for static rom files
typedef struct {
    const char *name;
    const char *content;
    bool isDirectory;
} S_FILE;

class Device {
public:
    char *label;
    char *devID;

    FATFS fatfs;
    uint8_t buff[FF_MAX_SS] = {};

    uint32_t blockCount;
    uint16_t blockSize;
    uint32_t totalSize;

    bool writeProtected = false;

    uint8_t *data;

    explicit Device(const char *label);

    Device(const char *label, uint32_t sizeInKb);

    ~Device();

    void init(unsigned int drvID);

//    void load();

//    void unload();

    bool ready();

    uint32_t read(uint8_t *buffer, uint32_t sector, uint32_t count);

    uint32_t write(uint8_t *buffer, uint32_t sector, uint32_t count);

    uint32_t read(uint8_t *buffer, uint32_t sector, uint32_t offset, uint32_t buff_size);

    uint32_t write(uint8_t *buffer, uint32_t sector, uint32_t offset, uint32_t buff_size);

    uint32_t ioctl(uint8_t devIndex, uint8_t const scsi_cmd[16], void *buffer, uint16_t buff_size);

    FRESULT mkFatFs(S_FILE *files[], int file_cnt);

    void publish_files(S_FILE files[], int file_cnt, bool overwrite);
};

#endif //JS_PICO_DEVICE_H
