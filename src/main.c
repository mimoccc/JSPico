//-----------------------------------------------------------------------------
// Main file, main routine
//-----------------------------------------------------------------------------

#include <pico/stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "bsp/board.h"
#include "tusb.h"
#include "board.h"
#include "msc_disk.h"
#include "hw/led.h"
#include "fs_data.c"
#include "js.h"
#include "log/log.h"
#include "fatfs/ff.h"
#include "tasks/tasks.h"
#include "tasks/task_main.h"
#include "tasks/task_js.h"

//-----------------------------------------------------------------------------

bool mounted = false;
bool suspended = true;
bool reboot = false;
bool ejected = false;

TaskHandle_t *t_handle;

struct js *js;
char *js_buff;

BYTE *ram_disk;
DWORD ram_disk_size = DISK_SIZE_KB * 2;
FATFS fat_fs;
unsigned char buff[DISK_BLOCK_SIZE] = {};

int main(void) {
    // set blink when loading
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
    bool msc_disk_prepared = msc_disk_init(
            DISK_IN_RAM,
            static_files,
            array_size(static_files)
    );
    // check disk state
    if (msc_disk_prepared) {
        // ok nothing
        set_led_blink_interval(BLINK_NONE);
        // log message
        tlog("-------------------------------------------------------------------------------");
        tlog("Board starting");
        tlog("-------------------------------------------------------------------------------");
        tlog("Virtual file system prepared.");
        // Get tasks and allocate handles
        tlog("Starting main application thread.");
        const int task_count = sizeof(TASKS) / sizeof(VTASK *);
        t_handle = malloc((task_count+2) * sizeof(VTASK));
        // Initialize all tasks one, to next
        for (int t1 = 0; t1 < task_count; t1++) {
            char *name = TASKS[t1]->task_name;
            slog("Initializing task : %s.", name);
            TASKS[t1]->task_init(NULL);
        }
        // Create task routines, start as subtasks in freertos
        for (int t2 = 0; t2 < task_count; t2++) {
            char *name = TASKS[t2]->task_name;
            slog("Starting task : %s.", name);
            xTaskCreate(
                    TASKS[t2]->task_main,
                    TASKS[t2]->task_name,
                    TASKS[t2]->stack_depth,
                    TASKS[t2]->parameters,
                    TASKS[t2]->priority,
                    &t_handle[t2]
            );
            slog("Task : %s started.", name);
        }
        vTaskStartScheduler();
        // Keep going until restart
        while (!reboot) {
            // should only get here if there is insufficient RAM
        }
    } else {
        // error start fast blinking
        set_led_blink_interval(BLINK_ERROR);
    }
    tlog("-------------------------------------------------------------------------------");
    tlog("Rebooting...");
    tlog("-------------------------------------------------------------------------------");
    free(t_handle);
    free(js);
    free(js_buff);
    free(ram_disk);
}

//-----------------------------------------------------------------------------