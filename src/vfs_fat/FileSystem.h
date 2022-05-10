#ifndef JS_PICO_FILESYSTEM_H
#define JS_PICO_FILESYSTEM_H

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

class FileSystem {
public:
    std::vector<Device *> devices = {};

    void onTudResume();

    void onTudSuspend(bool b);

    void onTudMount();

    void onTudUmount();

    void addDevice(Device *device);

    void getDeviceVendor(uint8_t devIndex, uint8_t *buffer);

    void getDeviceProduct(uint8_t devIndex, uint8_t *buffer);

    void getDeviceRevision(uint8_t devIndex, uint8_t *buffer);

    bool isDeviceReady(uint8_t devIndex);

    bool isDeviceReadOnly(uint8_t devIndex);
};

#endif //JS_PICO_FILESYSTEM_H
