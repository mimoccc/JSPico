#include "RamDisk.h"

RamDisk::RamDisk() : Device(RAM) {}

RamDisk::~RamDisk() {
    Device::~Device();
}