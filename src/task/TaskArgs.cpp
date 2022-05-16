#include "TaskArgs.h"

TaskArgs::TaskArgs(char *sender, char* destination) {
    uint s_size = strlen(sender);
    this->sender = static_cast<char *>(malloc(s_size));
    memcpy(this->sender, sender, s_size);
    s_size = strlen(destination);
    this->destination = static_cast<char *>(malloc(s_size));
    memcpy(this->destination, destination, s_size);
}

TaskArgs::~TaskArgs() {
    free(sender);
}
