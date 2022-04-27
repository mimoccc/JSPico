//-----------------------------------------------------------------------------
// Mass storage disc
//-----------------------------------------------------------------------------
bool disk_prepare();
// file structure for import to device
// static rom files
typedef struct  {
    char* name;
    char* content;
} S_FILE;
//-----------------------------------------------------------------------------