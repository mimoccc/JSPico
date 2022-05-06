//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#include "log/log.h"
#include "tusb.h"
#include "msc_disk.h"
#include "hw/led.h"
#include "fs_data.c"

//-----------------------------------------------------------------------------

void task_main_init() {
    tlog("Main task init start.");
    // -
    tlog("Main task initialized.");
}

//-----------------------------------------------------------------------------

void task_main_task() {
    tlog("Main task cycle called.");
    // msc disk task
    msc_disk_task();
    // led task
    led_task();
}

//-----------------------------------------------------------------------------