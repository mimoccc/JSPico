#ifndef JS_PICO_FLASH_DISK_H
#define JS_PICO_FLASH_DISK_H

#include "Device.h"

static char const *FLASH = "FLASH";

class FlashDisk : public Device {
public:
    FlashDisk();

    ~FlashDisk();
};

#endif //JS_PICO_FLASH_DISK_H
