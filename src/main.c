//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bsp/board.h"
#include "tusb.h"
//--------------------------------------------------------------------------------------------------
void cdc_task(void);
//--------------------------------------------------------------------------------------------------
int main(void) {
    board_init();
    tusb_init();
    while (1) {
        tud_task();
        cdc_task();
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
void tud_mount_cb(void) {
}
//--------------------------------------------------------------------------------------------------
void tud_umount_cb(void){
}
//--------------------------------------------------------------------------------------------------
void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
}
//--------------------------------------------------------------------------------------------------
void tud_resume_cb(void) {
}
//--------------------------------------------------------------------------------------------------
void cdc_task(void) {
    if (tud_cdc_available()) {
        char buf[64];
        uint32_t count = tud_cdc_read(buf, sizeof(buf));
        (void) count;
        // echoes back
        tud_cdc_write(buf, count);
        tud_cdc_write_flush();
    }
}
//--------------------------------------------------------------------------------------------------
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
    (void) itf;
    (void) rts;
    if (dtr) {
        // Terminal connected
    } else {
        // Terminal disconnected
    }
}
//--------------------------------------------------------------------------------------------------
void tud_cdc_rx_cb(uint8_t itf) {
    (void) itf;
}
//--------------------------------------------------------------------------------------------------