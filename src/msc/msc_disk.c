//-----------------------------------------------------------------------------
// Mass storage disc
//-----------------------------------------------------------------------------
#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "flash/flash.h"
#include "board.h"
//-----------------------------------------------------------------------------
#if CFG_TUD_MSC
//-----------------------------------------------------------------------------
// Disc data
#include "msc_disk_data.c"
//-----------------------------------------------------------------------------
// Eject state
static bool ejected = false;
// buffer
char vfs_buff[4096] = {};
//-----------------------------------------------------------------------------
void tud_msc_inquiry_cb(
    uint8_t lun, 
    uint8_t vendor_id[8], 
    uint8_t product_id[16], 
    uint8_t product_rev[4]
) {
    (void) lun;
    char vid[] = "MJDEV      ";
    const char pid[] = "Mass Storage";
    const char rev[] = "1.0";
    memcpy(vid, DISK_NAME, strlen(DISK_NAME));
    memcpy(vendor_id  , vid, strlen(vid));
    memcpy(product_id , pid, strlen(pid));
    memcpy(product_rev, rev, strlen(rev));
}
//-----------------------------------------------------------------------------
bool tud_msc_test_unit_ready_cb(uint8_t lun) {
    (void) lun;
    if (ejected) {
        tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3a, 0x00);
        return false;
    }
    return true;
}
//-----------------------------------------------------------------------------
void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size) {
    (void) lun;
    *block_count = DISK_BLOCK_NUM;
    *block_size  = DISK_BLOCK_SIZE;
}
//-----------------------------------------------------------------------------
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject) {
    (void) lun;
    (void) power_condition;
    if (load_eject) {
        if (start) {
            // load disk storage
            ejected = false;
        } else {
            // unload disk storage
            ejected = true;
        }
    }
    return true;
}
//-----------------------------------------------------------------------------
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
    (void) lun;
    if (lba >= DISK_BLOCK_NUM)
        return -1;
    uint8_t const* addr = device_data_ptr + (DISK_BLOCK_SIZE * lba) + offset;
    memcpy(buffer, addr, bufsize);
    return (int32_t) bufsize;
}
//-----------------------------------------------------------------------------
bool tud_msc_is_writable_cb(uint8_t lun) {
    (void) lun;
    return true;
}
//-----------------------------------------------------------------------------
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
    (void) lun;
    if (lba >= DISK_BLOCK_NUM)
        return -1;
    uint8_t const* addr = device_data_ptr + (DISK_BLOCK_SIZE * lba) + offset;
    memcpy(addr, buffer, bufsize);
    return (int32_t) bufsize;
}
//-----------------------------------------------------------------------------
int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize) {
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
//-----------------------------------------------------------------------------