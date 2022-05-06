//-----------------------------------------------------------------------------
// Main file, main routine
//-----------------------------------------------------------------------------

#include <pico/stdlib.h>
#include "bsp/board.h"
#include "tusb.h"
#include "board.h"
#include "msc_disk.h"
#include "hw/led.h"
#include "fs_data.c"
#include "js.h"
#include "log/log.h"
#include "fatfs/ff.h"
#include "tasks/stask.h"
#include "tasks/task_js.h"
#include "tasks/task_main.h"
#include "boot/boot.h"

//-----------------------------------------------------------------------------

bool mounted = false;
bool suspended = true;
bool reboot = false;
bool ejected = false;

struct js *js;
char *js_buff;
BYTE *ram_disk;
DWORD ram_disk_size = DISK_SIZE_KB * 2;
FATFS fat_fs;
unsigned char buff[DISK_BLOCK_SIZE] = {};
TASK *TASKS[] = {
        &TASK_MAIN,
        &TASK_JS
};
int currentTaskIdx = 0;

/**
 * Initialize all task at once
 */
void initTasks() {
    // task count
    const int task_count = sizeof(TASKS) / sizeof(TASK *);
    // Initialize all tasks one, to next
    for (int t1 = 0; t1 < task_count; t1++) {
        char *name = TASKS[t1]->task_name;
        slog("Initializing task : %s.", name);
        TASKS[t1]->task_init(TASKS[t1]->task_init_parameters);
    }
}

// call next scheduled task in cycle
void nextTask() {
    // task count
    const int task_count = sizeof(TASKS) / sizeof(TASK *);
    // check task index and reset if over
    if (currentTaskIdx >= task_count) currentTaskIdx = 0;
    // current task
    TASK *currentTask = TASKS[currentTaskIdx];
    // if task is not in sleep
    if (!currentTask->sleep) {
        // current board millis
        uint32_t bd_millis = board_millis();
        // scheduled task millis
        uint32_t t_millis = currentTask->schedule_ms;
        // current task priority
        uint16_t t_priority = currentTask->priority + 1; // min 1
        // if task is about to be runned
        if (bd_millis >= t_millis) {
            // start current task
            currentTask->task_task(currentTask->task_task_parameters);
            // next schedule
            t_millis = board_millis() + (t_priority * 10);
            // set back to tasks
            currentTask->schedule_ms = t_millis;
        }
        // increment index of task
        currentTaskIdx = currentTaskIdx + 1;
    }
}

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
        initTasks();
        // Keep going until restart
        while (!reboot) {
            // check boot sel button and reboot if pressed 500ms
            check_for_boot_sel_reset();
            // tiny usb device task
            tud_task();
            // run next task from tasks
            nextTask();
        }
    } else {
        // error start fast blinking
        set_led_blink_interval(BLINK_ERROR);
    }
    tlog("-------------------------------------------------------------------------------");
    tlog("Rebooting...");
    tlog("-------------------------------------------------------------------------------");
    free(js);
    free(js_buff);
    free(ram_disk);
}

//-----------------------------------------------------------------------------