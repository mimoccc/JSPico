#ifndef JS_PICO_FILE_SYSTEM_H
#define JS_PICO_FILE_SYSTEM_H

#include <pico/bootrom.h>
#include <pico/stdlib.h>
#include "Device.h"
#include <cassert>
#include <cinttypes>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "ffconf.h"
#include "ff.h"
#include "diskio.h"

class FileSystem {
public:
    static std::vector<Device *> devices;

    FileSystem();

    ~FileSystem();

    static void onTudResume();

    static void onTudSuspend(bool vanWakeUp);

    static void onTudMount();

    static void onTudUmount();

    static void addDevice(Device *device);

    static uint16_t deviceCount();

    static void getDeviceVendor(uint8_t devIndex, uint8_t *buffer);

    static void getDeviceProduct(uint8_t devIndex, uint8_t *buffer);

    static void getDeviceRevision(uint8_t devIndex, uint8_t *buffer);

    static bool isDeviceReady(uint8_t devIndex);

    static bool isDeviceReadOnly(uint8_t devIndex);
};

#endif //JS_PICO_FILE_SYSTEM_H
