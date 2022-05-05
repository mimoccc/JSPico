//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#include "bsp/board.h"
#include "pico/bootrom.h"
#include "log/log.h"

//-----------------------------------------------------------------------------

// property for to reboot device from js code, or by call
extern bool reboot;

//-----------------------------------------------------------------------------

// Set led state
void board_set_led(int value) {
    board_led_write(value);
}

// Reboot device
// todo
void board_reset() {
    tlog("Function board.reset() is not yet implemented.");
    reboot = true;
}

// Reboot device to flash mode
void board_factory_reset() {
    tlog("Function board.factory() called. Rebooting to boot-sel.");
    reset_usb_boot(0, 0);
}

//-----------------------------------------------------------------------------