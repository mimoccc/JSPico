//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef JSPICO_TASKS_H
#define JSPICO_TASKS_H

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
#include "tasks/stask.h"
#include "task_main.h"
#include "task_js.h"

//-----------------------------------------------------------------------------

VTASK* TASKS[] = {
        &TASK_MAIN,
        &TASK_JS
};

//-----------------------------------------------------------------------------

#endif //JSPICO_TASKS_H

//-----------------------------------------------------------------------------