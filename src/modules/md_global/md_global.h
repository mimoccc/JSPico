//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef JSPICO_MODULE_GLOBAL_H
#define JSPICO_MODULE_GLOBAL_H

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
#include "global.h"

//-----------------------------------------------------------------------------

JS_FNC GLOBAL_FUNCTIONS[] = {
        {
                .name = "delay",
                .params = "vi",
                .fn_pointer = (uintptr_t) delay
        }
};

//-----------------------------------------------------------------------------

JS_MODULE JS_GLOBAL = {
        .mount_point = NULL,
        .functions = GLOBAL_FUNCTIONS,
        .fnc_count = array_size(GLOBAL_FUNCTIONS)
};

//-----------------------------------------------------------------------------

#endif //JSPICO_MODULE_CONSOLE_H

//-----------------------------------------------------------------------------