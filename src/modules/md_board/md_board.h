//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef JSPICO_MODULE_BOARD_H
#define JSPICO_MODULE_BOARD_H

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
#include "board.h"

//-----------------------------------------------------------------------------

JS_FNC BOARD_FUNCTIONS[] = {
        {
                .name = "set_led",
                .params = "vi",
                .fn_pointer = (uintptr_t) board_set_led
        },
        {
                .name = "reset",
                .params = "v",
                .fn_pointer = (uintptr_t) board_reset
        },
        {
                .name = "factory_reset",
                .params = "v",
                .fn_pointer = (uintptr_t) board_factory_reset
        }
};

//-----------------------------------------------------------------------------

JS_MODULE JS_BOARD = {
        .name = "board",
        .mount_point = NULL,
        .functions = BOARD_FUNCTIONS,
        .fnc_count = array_size(BOARD_FUNCTIONS)
};

//-----------------------------------------------------------------------------

#endif //JSPICO_MODULE_BOARD_H

//-----------------------------------------------------------------------------
