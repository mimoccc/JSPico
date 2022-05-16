#include "JSPico.h"
#include "Device.h"

static JSPico *js_pico = new JSPico();

// main start whole device process
int main() {
    js_pico->init();
    js_pico->start();
}

// -- C callbacks -------------------------------------------------------------

// mount device for usb
void tud_mount_cb() {
   FileSystem::onTudMount();
}

// unmount device for usb
void tud_umount_cb() {
    FileSystem::onTudUmount();
}

// suspend device
void tud_suspend_cb(bool remoteWakeupEn) {
    FileSystem::onTudSuspend(remoteWakeupEn);
}

// make device alive
void tud_resume_cb() {
    FileSystem::onTudResume();
}

// return device count to usb
uint8_t tud_msc_get_maxlun_cb() {
    return FileSystem::deviceCount();
}

// return device details to usb
void tud_msc_inquiry_cb(uint8_t devIndex, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
    FileSystem::getDeviceVendor(devIndex, vendor_id);
    FileSystem::getDeviceProduct(devIndex, product_id);
    FileSystem::getDeviceRevision(devIndex, product_rev);
}

// return device state ready to usb
bool tud_msc_test_unit_ready_cb(uint8_t devIndex) {
    return FileSystem::isDeviceReady(devIndex);
}

// return device state readonly to usb
bool tud_msc_is_writable_cb(uint8_t devIndex) {
    return !FileSystem::isDeviceReadOnly(devIndex);
}

// return device capcity to usb
void tud_msc_capacity_cb(uint8_t devIndex, uint32_t *block_count, uint16_t *block_size) {
    *block_count = FileSystem::devices[devIndex]->blockCount;
    *block_size = FileSystem::devices[devIndex]->blockSize;
}

// stop / start device usb command
bool tud_msc_start_stop_cb(uint8_t devIndex, uint8_t power_condition, bool start, bool load_eject) {
    if (load_eject) {
        if (start) {
            //FileSystem::devices[devIndex]->load();
        } else {
            //FileSystem::devices[devIndex]->unload();
        }
    }
    return true;
}

// read device from usb
int32_t tud_msc_read10_cb(uint8_t devIndex, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t buff_size) {
    Device *device = FileSystem::devices[devIndex];
    return device->read(buffer, lba, offset, buff_size);
}

// write to device from usb
int32_t tud_msc_write10_cb(uint8_t devIndex, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t buff_size) {
    Device *device = FileSystem::devices[devIndex];
    return device->write(buffer, lba, offset, buff_size);
}

// ioct from usb
int32_t tud_msc_scsi_cb(uint8_t devIndex, uint8_t const scsi_cmd[16], void *buffer, uint16_t buff_size) {
    Device *device = FileSystem::devices[devIndex];
    return device->ioctl(devIndex, scsi_cmd, buffer, buff_size);
}

// -- C callbacks -------------------------------------------------------------