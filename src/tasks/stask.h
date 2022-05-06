//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef JSPICO_S_TASK_H
#define JSPICO_S_TASK_H

//-----------------------------------------------------------------------------

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------

typedef void (*TASK_FNC)(void *);

typedef struct {
    char *task_name;            // task name
    void *task_init_parameters; // params for initialize function
    TASK_FNC task_init;         // pointer to function that initialize task
    void *task_task_parameters; // task cycle parameters
    TASK_FNC task_task;         // pointer to task loop function
    uint32_t schedule_ms;       // next task cycle function run schedule
    uint8_t priority;           // task priority 1-max 255 - min
    bool sleep;                 // task is halted, sleeping
} TASK;

//-----------------------------------------------------------------------------

#endif //JSPICO_S_TASK_H

//-----------------------------------------------------------------------------