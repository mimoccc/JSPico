//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef JSPICO_MODULE_GPIO_H
#define JSPICO_MODULE_GPIO_H

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
#include "gpio.h"

//-----------------------------------------------------------------------------

JS_FNC GPIO_FUNCTIONS[] = {
};

//-----------------------------------------------------------------------------

JS_MODULE JS_GPIO = {
        .mount_point = NULL,
        .name = "gpio",
        .functions = GPIO_FUNCTIONS,
        .fnc_count = array_size(GPIO_FUNCTIONS)
};

//-----------------------------------------------------------------------------

#endif //JSPICO_MODULE_GPIO_H

//-----------------------------------------------------------------------------