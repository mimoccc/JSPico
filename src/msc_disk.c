//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h"
#include "hardware/flash.h"
//--------------------------------------------------------------------------------------------------
#if CFG_TUD_MSC
//--------------------------------------------------------------------------------------------------
// We're going to erase and reprogram a region 15MB from the start of flash.
// Once done, we can access this at XIP_BASE + 15MB.
#define FLASH_TARGET_OFFSET (15 * 1024 * 1024)
//--------------------------------------------------------------------------------------------------
// Flash memory index
const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
//--------------------------------------------------------------------------------------------------
// Eject state
static bool ejected = false;
//--------------------------------------------------------------------------------------------------
// Disc data
#include "disc_data.c"
//--------------------------------------------------------------------------------------------------
// Invoked when received SCSI_CMD_INQUIRY
// Application fill vendor id, product id and revision with string up to 8, 16, 4 characters respectively
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
    (void) lun;
    char vid[] = "           ";
    const char pid[] = "Mass Storage";
    const char rev[] = "1.0";
    memcpy(vid, DEV_NAME, strlen(DEV_NAME));
    memcpy(vendor_id  , vid, strlen(vid));
    memcpy(product_id , pid, strlen(pid));
    memcpy(product_rev, rev, strlen(rev));
}
//--------------------------------------------------------------------------------------------------
bool tud_msc_test_unit_ready_cb(uint8_t lun) {
    (void) lun;
    if (ejected) {
        tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3a, 0x00);
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------------------------
void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size) {
    (void) lun;
    *block_count = DISK_BLOCK_NUM;
    *block_size  = DISK_BLOCK_SIZE;
}
//--------------------------------------------------------------------------------------------------
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject) {
    (void) lun;
    (void) power_condition;
    if ( load_eject ) {
        if (start) {
            // load disk storage
        } else {
            // unload disk storage
            ejected = true;
        }
    }
    return true;
}
//--------------------------------------------------------------------------------------------------
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
    (void) lun;
    if (lba >= DISK_BLOCK_NUM)
        return -1;
    uint8_t const* addr = msc_disk[lba] + offset;
    memcpy(buffer, addr, bufsize);
    return (int32_t) bufsize;
}
//--------------------------------------------------------------------------------------------------
bool tud_msc_is_writable_cb (uint8_t lun) {
    (void) lun;
    #ifdef CFG_EXAMPLE_MSC_READONLY
    return false;
    #else
    return true;
    #endif
}
//--------------------------------------------------------------------------------------------------
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
    (void) lun;
    if (lba >= DISK_BLOCK_NUM)
        return -1;
    #ifndef CFG_EXAMPLE_MSC_READONLY
    uint8_t* addr = msc_disk[lba] + offset;
    memcpy(addr, buffer, bufsize);
    #else
    (void) lba; (void) offset; (void) buffer;
    #endif
    return (int32_t) bufsize;
}
//--------------------------------------------------------------------------------------------------
int32_t tud_msc_scsi_cb (uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize) {
    void const* response = NULL;
    int32_t resplen = 0;
    bool in_xfer = true;
    switch (scsi_cmd[0]) {
        default:
            tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);
            resplen = -1;
            break;
    }
    if (resplen > bufsize)
        resplen = bufsize;
    if (response && (resplen > 0)) {
        if(in_xfer) {
            memcpy(buffer, response, (size_t) resplen);
        } else {
            // SCSI output
        }
    }
    return (int32_t) resplen;
}
#endif
//--------------------------------------------------------------------------------------------------