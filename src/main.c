//-----------------------------------------------------------------------------
// Main file, main routine
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bsp/board.h"
#include "tusb.h"
#include "board.h"
//-----------------------------------------------------------------------------
bool mounted = false;
bool suspended = true;
bool restart = false;
//-----------------------------------------------------------------------------
enum {
    BLINK_UNMOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500,
};
//-----------------------------------------------------------------------------
static uint32_t blink_interval_ms = BLINK_UNMOUNTED;
//-----------------------------------------------------------------------------
void led_blinking_task(void);
//-----------------------------------------------------------------------------
int main(void) {
    board_init();
    target_board_init();
    tusb_init();
    while (!restart) {
        tud_task();
        led_blinking_task();
    }
    return 0;
}
//-----------------------------------------------------------------------------
void tud_mount_cb(void) {
    mounted = true;
    blink_interval_ms = BLINK_MOUNTED;
}
//-----------------------------------------------------------------------------
void tud_umount_cb(void) {
    mounted = false;
    blink_interval_ms = BLINK_UNMOUNTED;
}
//-----------------------------------------------------------------------------
void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
    suspended = true;
    blink_interval_ms = BLINK_SUSPENDED;
}
//-----------------------------------------------------------------------------
void tud_resume_cb(void) {
    suspended = false;
    blink_interval_ms = BLINK_MOUNTED;
}
//-----------------------------------------------------------------------------
void led_blinking_task(void) {
    static uint32_t start_ms = 0;
    static bool led_state = false;
    if (board_millis() - start_ms < blink_interval_ms) return; // not enough time
    start_ms += blink_interval_ms;
    board_led_write(led_state);
    led_state = 1 - led_state;
}
//-----------------------------------------------------------------------------