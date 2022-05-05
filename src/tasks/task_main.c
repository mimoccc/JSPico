//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#include <pico/stdlib.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include "board.h"
#include "log/log.h"
#include "boot/boot.h"
#include "tusb.h"
#include "msc/msc_disk.h"
#include "hw/led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

//-----------------------------------------------------------------------------

void task_main_init() {
    tlog("Main task init start.");
    // -
    tlog("Main task initialized.");
}

//-----------------------------------------------------------------------------

void task_main_task() {
    tlog("Main task is really started.");
    // small delay before loop
    vTaskDelay(10);
    while (!reboot) {
        // check boot sel button and reboot if pressed 500ms
        check_for_boot_sel_reset();
        // tiny usb device task
        tud_task();
        // msc disk task
        msc_disk_task();
        // led task
        led_task();
    }
}

//-----------------------------------------------------------------------------