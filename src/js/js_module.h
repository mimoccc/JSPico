//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef JSPICO_JS_MODULE_H
#define JSPICO_JS_MODULE_H

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

typedef struct {
    char *name;
    char *params;
    uintptr_t fn_pointer;
} JS_FNC;

//-----------------------------------------------------------------------------

typedef struct {
    char *name;
    char *mount_point;
    JS_FNC* functions;
    int fnc_count;
} JS_MODULE;

//-----------------------------------------------------------------------------

#endif //JSPICO_JS_MODULE_H

//-----------------------------------------------------------------------------