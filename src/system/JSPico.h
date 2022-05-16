#ifndef JS_PICO_JS_PICO_H
#define JS_PICO_JS_PICO_H

#include "FileSystem.h"
#include "TaskManager.h"
#include "BlinkTask.h"
#include "BootSelTask.h"

class JSPico {
public:
    TaskManager *taskManager;
    FileSystem *fileSystem;

    void init();

    void start();

    void reboot();

    static void rebootRecovery();
};

#endif //JS_PICO_JS_PICO_H