#include "FlashDisk.h"

FlashDisk::FlashDisk() : Device(FLASH) {}

FlashDisk::~FlashDisk() {
    Device::~Device();
}