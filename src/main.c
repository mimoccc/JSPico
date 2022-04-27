//-----------------------------------------------------------------------------
// Main file, main routine
//-----------------------------------------------------------------------------

#include <pico/stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bsp/board.h"
#include "tusb.h"
#include "board.h"
#include "tusb_config.h"
#include "msc_disk.h"
#include "boot/boot.h"
#include "hw/led.h"

#include "fs_data.c"

//-----------------------------------------------------------------------------

int main(void) {
    set_led_blink_interval(BLINK_STARTING);
    // board init
    board_init();
    // standard io init
    stdio_init_all();
    // custom board initialization
    target_board_init();
    // init usb
    tusb_init();
    // prepare ram/flash disk
    bool msc_disk_prepared = msc_disk_init(DISK_IN_RAM, static_files);
    // check disk state
    if (msc_disk_prepared) {
        // ok nothing
        set_led_blink_interval(BLINK_NONE);
    } else {
        // error start fast blinking
        set_led_blink_interval(BLINK_ERROR);
    }
    #pragma clang diagnostic push
    #pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        // check boot sel button and reboot if pressed 500ms
        check_for_boot_sel_reset();
        // tiny usb device task
        tud_task();
        // msc disk task
        msc_disk_task();
        // led task
        led_task();
    }
    #pragma clang diagnostic pop
}

//-----------------------------------------------------------------------------