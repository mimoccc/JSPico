#include "JSPico.h"
#include "Device.h"
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "tusb.h"

static JSPico *js_pico = new JSPico();

// main start whole device process
int main() {
    js_pico->init();
    js_pico->start();
}

// -- C callbacks -------------------------------------------------------------

void tud_mount_cb() {
    js_pico->onTudMount();
}

void tud_umount_cb() {
    js_pico->onTudUmount();
}

void tud_suspend_cb(bool remoteWakeupEn) {
    js_pico->onTudSuspend(remoteWakeupEn);
}

void tud_resume_cb() {
    js_pico->onTudResume();
}

uint8_t tud_msc_get_maxlun_cb() {
    return js_pico->fileSystem->devices.size();
}

void tud_msc_inquiry_cb(uint8_t devIndex, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
    js_pico->fileSystem->getDeviceVendor(devIndex, vendor_id);
    js_pico->fileSystem->getDeviceProduct(devIndex, product_id);
    js_pico->fileSystem->getDeviceRevision(devIndex, product_rev);
}

bool tud_msc_test_unit_ready_cb(uint8_t devIndex) {
    return js_pico->fileSystem->isDeviceReady(devIndex);
}

bool tud_msc_is_writable_cb(uint8_t devIndex) {
    return !js_pico->fileSystem->isDeviceReadOnly(devIndex);
}

void tud_msc_capacity_cb(uint8_t devIndex, uint32_t *block_count, uint16_t *block_size) {
    *block_count = DISK_BLOCK_NUM;//js_pico->fileSystem->devices[devIndex]->blockCount;
    *block_size = DISK_BLOCK_SIZE;//js_pico->fileSystem->devices[devIndex]->blockSize;
}

bool tud_msc_start_stop_cb(uint8_t devIndex, uint8_t power_condition, bool start, bool load_eject) {
    if (load_eject) {
        if (start) {
            js_pico->fileSystem->devices[devIndex]->load();
        } else {
            js_pico->fileSystem->devices[devIndex]->unload();
        }
    }
    return true;
}

int32_t tud_msc_read10_cb(uint8_t devIndex, uint32_t lba, uint32_t offset, void *buffer, uint32_t buff_size) {
    return js_pico->fileSystem->devices[devIndex]->read(buffer, lba, offset, buff_size);
}

int32_t tud_msc_write10_cb(uint8_t devIndex, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t buff_size) {
    return js_pico->fileSystem->devices[devIndex]->write(buffer, lba, offset, buff_size);
}

int32_t tud_msc_scsi_cb(uint8_t devIndex, uint8_t const scsi_cmd[16], void *buffer, uint16_t buff_size) {
    return js_pico->fileSystem->devices[devIndex]->ioctl(devIndex, scsi_cmd, buffer, buff_size);
}

// -- C callbacks -------------------------------------------------------------