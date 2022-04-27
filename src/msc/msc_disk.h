//-----------------------------------------------------------------------------
// Mass storage disc
//-----------------------------------------------------------------------------

#ifndef MSC_DISK_H
#define MSC_DISK_H

// file structure for static rom files
typedef struct  {
    char* name;
    char* content;
    bool isDirectory;
} S_FILE;

typedef enum  {
    DISK_IN_RAM = 0,
    DISK_IN_FLASH = 1
} DISK_TYPE;

bool msc_disk_init(DISK_TYPE type, S_FILE files[]);

void msc_disk_task();

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------