//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#include <string.h>
#include "fatfs/ff.h"

//-----------------------------------------------------------------------------

char *log_file = "js.log";

//-----------------------------------------------------------------------------

void console_log_buff(char* buff, int size) {
    FRESULT f_res;
    FIL f;
    UINT bw;
    f_res = f_open(&f, log_file, FA_WRITE | FA_OPEN_ALWAYS | FA_OPEN_APPEND);
    if (f_res == FR_OK) {
        f_write(&f, buff, size, &bw);
        f_write(&f, "\n", 1, &bw);
        f_close(&f);
    }
}

//-----------------------------------------------------------------------------

void console_log(char *message) {
    console_log_buff(message, strlen(message));
}

//-----------------------------------------------------------------------------