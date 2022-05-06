//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//#include <assert.h>
//#include <inttypes.h>
//#include <math.h>
//#include <stdarg.h>
//#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "pico/stdlib.h"
#include "bsp/board.h"
//#include "board.h"
//#include <stdint.h>
#include "tasks/stask.h"
#include "main.h"

//-----------------------------------------------------------------------------

void delay(uint32_t millis) {
    TASK *currentTask = TASKS[currentTaskIdx];
    currentTask->sleep = true;
    uint32_t start_ms = board_millis();
    while (board_millis() - start_ms < millis) {
        nextTask();
    }
    currentTask->sleep = false;
}

//-----------------------------------------------------------------------------