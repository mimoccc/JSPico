//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef JSPICO_MODULE_FS_H
#define JSPICO_MODULE_FS_H

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
#include "fs.h"

//-----------------------------------------------------------------------------

JS_FNC FS_FUNCTIONS [] = {
};

//-----------------------------------------------------------------------------

JS_MODULE JS_FS = {
        .mount_point = NULL,
        .name = "fs",
        .functions = FS_FUNCTIONS,
        .fnc_count = array_size(FS_FUNCTIONS)
};

//-----------------------------------------------------------------------------

#endif //JSPICO_MODULE_FS_H

//-----------------------------------------------------------------------------