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

    void start() const;

    void reboot() const;

    static void rebootRecovery();

    void onTudMount() const;

    void onTudUmount() const;

    void onTudSuspend(bool b) const;

    void onTudResume() const;

    uint8_t getDeviceCount();
};

#endif //JS_PICO_JS_PICO_H