//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef JSPICO_MODULES_H
#define JSPICO_MODULES_H

//-----------------------------------------------------------------------------

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "js_module.h"
#include "modules/md_global/md_global.h"
#include "modules/md_board/md_board.h"
#include "modules/md_console/md_console.h"
#include "modules/md_gpio/md_gpio.h"
#include "modules/md_fs/md_fs.h"

//-----------------------------------------------------------------------------

JS_MODULE* JS_MODULES[] = {
        &JS_BOARD,
        &JS_GLOBAL,
        &JS_CONSOLE,
        &JS_GPIO,
        &JS_FS
};

//-----------------------------------------------------------------------------

#endif //JSPICO_MODULES_H


//-----------------------------------------------------------------------------