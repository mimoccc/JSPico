#ifndef JS_PICO_RAM_DISK_H
#define JS_PICO_RAM_DISK_H

#include "Device.h"

static char const * RAM = "RAM";

class RamDisk : public Device {
public:
    RamDisk();

    ~RamDisk();
};

#endif //JS_PICO_RAM_DISK_H