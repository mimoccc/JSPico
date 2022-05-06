//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef JSPICO_MODULE_CONSOLE_H
#define JSPICO_MODULE_CONSOLE_H

//-----------------------------------------------------------------------------

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "js/js_module.h"
#include "console.h"

//-----------------------------------------------------------------------------

JS_FNC CONSOLE_FUNCTIONS[] = {
        {
                .name = "log",
                .params = "vs",
                .fn_pointer = (uintptr_t) console_log
        }
};

//-----------------------------------------------------------------------------

JS_MODULE JS_CONSOLE = {
        .name = "console",
        .mount_point = NULL,
        .functions = CONSOLE_FUNCTIONS,
        .fnc_count = array_size(CONSOLE_FUNCTIONS)
};

//-----------------------------------------------------------------------------

#endif //JSPICO_MODULE_CONSOLE_H

//-----------------------------------------------------------------------------