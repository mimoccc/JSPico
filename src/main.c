//-----------------------------------------------------------------------------
// Main file, main routine
//-----------------------------------------------------------------------------

#include <hardware/gpio.h>
#include <hardware/regs/psm.h>
#include <hardware/structs/ioqspi.h>
#include <hardware/structs/sio.h>
#include <hardware/sync.h>
#include <pico/bootrom.h>
#include <pico/critical_section.h>
#include <pico/stdlib.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bsp/board.h"
#include "tusb.h"
#include "board.h"
#include "tusb_config.h"
#include "msc_disk.h"

//-----------------------------------------------------------------------------

const uint CS_PIN_INDEX = 1;

static bool mounted = false;
static bool suspended = true;
static bool restart = false;
static bool prepared = false;

//-----------------------------------------------------------------------------

enum {
    BLINK_NONE = 0,
    BLINK_ERROR = 50,
//    BLINK_UNMOUNTED = 250,
//    BLINK_MOUNTED = 1000,
//    BLINK_SUSPENDED = 2500,
};

//-----------------------------------------------------------------------------

static uint32_t blink_interval_ms = BLINK_NONE;

//-----------------------------------------------------------------------------

void led_blinking_task() {
    if (blink_interval_ms != BLINK_NONE) {
        static uint32_t start_ms = 0;
        static bool led_state = false;
        if (board_millis() - start_ms < blink_interval_ms)
            return;
        start_ms += blink_interval_ms;
        board_led_write(led_state);
        led_state = 1 - led_state;
    }
}

//-----------------------------------------------------------------------------

void set_chip_select_override(const enum gpio_override override) {
    hw_write_masked(&ioqspi_hw->io[CS_PIN_INDEX].ctrl,
                    override << IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_LSB,
                    IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_BITS);
}

//-----------------------------------------------------------------------------

bool __no_inline_not_in_flash_func(get_boot_sel_button)() {
    set_chip_select_override(GPIO_OVERRIDE_LOW);
    for (unsigned int i = 0; i < 1000; ++i) {
        __asm("nop");
    };
    bool button_state = !(sio_hw->gpio_hi_in & (1u << CS_PIN_INDEX));
    set_chip_select_override(GPIO_OVERRIDE_NORMAL);
    return button_state;
}

//-----------------------------------------------------------------------------

void tud_mount_cb(void) {
    mounted = true;
}

//-----------------------------------------------------------------------------

void tud_umount_cb(void) {
    mounted = false;
}

//-----------------------------------------------------------------------------

void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
    suspended = true;
}

//-----------------------------------------------------------------------------

void tud_resume_cb(void) {
    suspended = false;
}

//-----------------------------------------------------------------------------
// Check if reset is called by pressing button for 500 ms
void check_for_boot_sel_reset() {
    if (get_boot_sel_button()) {
        sleep_ms(500);
        if (get_boot_sel_button()) {
            reset_usb_boot(0, 0);
        }
    }
}

//-----------------------------------------------------------------------------

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#pragma ide diagnostic ignored "ConstantConditionsOC"
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
int main(void) {
    board_init();
    stdio_init_all();
    target_board_init();
    prepared = disk_prepare();
    if (prepared) {
        tusb_init();
        blink_interval_ms = BLINK_NONE;
    } else {
        blink_interval_ms = BLINK_ERROR;
    }
    while (!restart) {
        if (prepared) {
            tud_task();
        } else {
            check_for_boot_sel_reset();
        }
        led_blinking_task();
    }
}

#pragma clang diagnostic pop

//-----------------------------------------------------------------------------