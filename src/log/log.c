//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#include "modules/md_console/console.h"
//#include <pico/stdlib.h>
//#include <stdlib.h>
#include <stdio.h>
//#include <string.h>

//-----------------------------------------------------------------------------

void ilog(char *tmp, int i) {
    char str_buf[256];
    sprintf(str_buf, tmp, i);
    console_log(str_buf);
}

//-----------------------------------------------------------------------------

void slog(char *tmp, char *str) {
    char str_buf[256];
    sprintf(str_buf, tmp, str);
    console_log(str_buf);
}

//-----------------------------------------------------------------------------

void tlog(char *tmp) {
    console_log(tmp);
}

//-----------------------------------------------------------------------------

void tlogb(char* buff, int size) {
    console_log_buff(buff, size);
}