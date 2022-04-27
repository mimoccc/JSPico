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

//-----------------------------------------------------------------------------

static bool restart = false;
static bool prepared = false;

//-----------------------------------------------------------------------------

int main(void) {
    set_led_blink_interval(BLINK_STARTING);
    // board init
    board_init();
    // standard io init
    stdio_init_all();
    // custom board initialization
    target_board_init();
    // prepare ram/flash disk
    prepared = disk_prepare();
    // if all ok
    if (prepared) {
        tusb_init();
        set_led_blink_interval(BLINK_NONE);
    } else {
        set_led_blink_interval(BLINK_ERROR);
    }
    while (!restart) {
        if (prepared) {
            tud_task();
        }
        led_task();
        check_for_boot_sel_reset();
    }
}

//-----------------------------------------------------------------------------