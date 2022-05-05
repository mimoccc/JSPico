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
#include "FreeRTOS.h"
#include "task.h"

//-----------------------------------------------------------------------------

typedef struct {
    TaskFunction_t task_init;
    TaskFunction_t task_main;
    char* task_name;
    configSTACK_DEPTH_TYPE stack_depth;
    void* parameters;
    UBaseType_t priority;
} VTASK;

//-----------------------------------------------------------------------------

#endif //JSPICO_S_TASK_H

//-----------------------------------------------------------------------------