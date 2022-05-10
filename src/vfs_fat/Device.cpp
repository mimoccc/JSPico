#include "Device.h"

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include <cassert>
#include <cinttypes>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

Device::Device(char const *label) {
    this->label = label;
}

Device::~Device() {
}

void Device::load() {
//    data = malloc(blockCount * blockSize);
}

void Device::unload() {
//    free(data);
}

int32_t Device::read(void *buffer, uint32_t cluster, uint32_t offset, uint32_t buff_size) {
    if (cluster >= blockCount)
        return -1;
    void *addr = data[cluster] + offset;
    memcpy(buffer, addr, buff_size);
    return buff_size;
}

int32_t Device::write(void *buffer, uint32_t cluster, uint32_t offset, uint32_t buff_size) {
    if (cluster >= blockCount)
        return -1;
    void *addr = data[cluster] + offset;
    memcpy(addr, buffer, buff_size);
    return buff_size;
}

int32_t Device::ioctl(uint8_t devIndex, const uint8_t *scsi_cmd, void *buffer, uint16_t buff_size) {
    // read10 & write10 has their own callback and MUST not be handled here
    void const *response = NULL;
    int32_t resplen = 0;
    // most scsi handled is input
    bool in_xfer = true;
    switch (scsi_cmd[0]) {
        case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
            // Host is about to read/write etc ... better not to disconnect disk
            resplen = 0;
            break;
        case SCSI_CMD_START_STOP_UNIT:
            // Host try to eject/safe remove/poweroff us. We could safely disconnect with disk storage, or go into lower power
            /* scsi_start_stop_unit_t const * start_stop = (scsi_start_stop_unit_t const *) scsi_cmd;
              // Start bit = 0 : low power mode, if load_eject = 1 : unmount disk storage as well
              // Start bit = 1 : Ready mode, if load_eject = 1 : mount disk storage
              start_stop->start;
              start_stop->load_eject;
             */
            resplen = 0;
            break;
        default:
            // Set Sense = Invalid Command Operation
            tud_msc_set_sense(devIndex, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);
            // negative means error -> tinyusb could stall and/or response with failed status
            resplen = -1;
            break;
    }
    // return resplen must not larger than bufsize
    if (resplen > buff_size) resplen = buff_size;
    if (resplen > 0) {
        if (in_xfer) {
            memcpy(buffer, response, resplen);
        } else {
            // SCSI output
        }
    }
    return resplen;
}
