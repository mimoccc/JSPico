//-----------------------------------------------------------------------------
//
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
#include "led.h"

//-----------------------------------------------------------------------------

static uint32_t blink_interval = BLINK_NONE;

//-----------------------------------------------------------------------------

void set_led_blink_interval(uint32_t blink_interval_ms) {
    blink_interval = blink_interval_ms;
}

void led_task() {
    if (blink_interval != BLINK_NONE) {
        static uint32_t start_ms = 0;
        static bool led_state = false;
        if (board_millis() - start_ms < blink_interval)
            return;
        start_ms += blink_interval;
        board_led_write(led_state);
        led_state = 1 - led_state;
    }
}

void led_on() {
    board_led_write(1);
}

void led_off() {
    board_led_write(0);
}

//-----------------------------------------------------------------------------