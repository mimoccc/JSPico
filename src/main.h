//-----------------------------------------------------------------------------

#ifndef JSPICO_MAIN_H
#define JSPICO_MAIN_H

//-----------------------------------------------------------------------------

//#include "bsp/board.h"
#include "tusb.h"
//#include "pico/stdlib.h"
//#include "hardware/flash.h"
//#include "flash/flash_disk.h"
#include "board.h"
#include <stdlib.h>
//#include <stdint.h>
#include <stdbool.h>
//#include <stdio.h>
#include <string.h>
#include <time.h>
#include "fatfs/ff.h"
//#include "diskio.h"
#include "msc_disk.h"
#include "utils/base64.h"

//-----------------------------------------------------------------------------

extern bool mounted;
extern bool suspended;
extern bool reboot;
extern bool ejected;
extern struct js *js;
extern char *js_buff;
extern BYTE *ram_disk;
extern DWORD ram_disk_size;
extern FATFS fat_fs;
extern bool ejected;
extern unsigned char buff[DISK_BLOCK_SIZE];

//-----------------------------------------------------------------------------

#endif //JSPICO_MAIN_H

//-----------------------------------------------------------------------------