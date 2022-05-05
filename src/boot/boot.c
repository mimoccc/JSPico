//-----------------------------------------------------------------------------
// Boot device functions
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

//-----------------------------------------------------------------------------

const uint CS_PIN_INDEX = 1;

//-----------------------------------------------------------------------------

static critical_section_t cs;

//-----------------------------------------------------------------------------

void critical_sec_init() {
    critical_section_init(&cs);
    critical_section_enter_blocking(&cs);
}

//-----------------------------------------------------------------------------

void critical_sec_done(){
    critical_section_exit(&cs);
    critical_section_deinit(&cs);
}

//-----------------------------------------------------------------------------

void set_chip_select_override(const enum gpio_override override) {
    hw_write_masked(&ioqspi_hw->io[CS_PIN_INDEX].ctrl,
                    override << IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_LSB,
                    IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_BITS);
}

//-----------------------------------------------------------------------------

bool __no_inline_not_in_flash_func(get_boot_sel_button)() {
    critical_sec_init();
    set_chip_select_override(GPIO_OVERRIDE_LOW);
    for (unsigned int i = 0; i < 1000; ++i) {
        __asm("nop");
    };
    bool button_state = !(sio_hw->gpio_hi_in & (1u << CS_PIN_INDEX));
    set_chip_select_override(GPIO_OVERRIDE_NORMAL);
    critical_sec_done();
    return button_state;
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