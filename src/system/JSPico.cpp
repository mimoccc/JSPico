#include <pico/bootrom.h>
#include <pico/stdlib.h>

#include "JSPico.h"

#include "WebServerTask.h"
#include "FileSystem.h"
#include "FlashDisk.h"
#include "RamDisk.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wwritable-strings"


#pragma clang diagnostic pop

// init device
void JSPico::init() {
    stdio_init_all();

    // create base file system
    fileSystem = new FileSystem();
    // add flash device
    fileSystem->addDevice(new FlashDisk());
    // add ram device
    fileSystem->addDevice(new RamDisk());

    // init task manager
    taskManager = new TaskManager();
    // enable reboot by pressing boot sel button
    taskManager->addTask(new BootSelTask(500));
    // blinking task used as state identification
    taskManager->addTask(new BlinkTask(1000));
    // web server task
    taskManager->addTask(new WebServerTask());
}

// Start main process
void JSPico::start() const {
    while (taskManager->isNotEmpty()) {
        taskManager->processNextTask();
    }
    rebootRecovery();
}

// Reboot device
void JSPico::reboot() const {
    // just clear all tasks, which continues to reboot
    taskManager->clear();
}

// Reboot device to recovery / flashing
void JSPico::rebootRecovery() {
    reset_usb_boot(0, 0);
}

// Invoked when device is mounted
void JSPico::onTudMount() const {
    fileSystem->onTudMount();
}

// Invoked when device is unmounted
void JSPico::onTudUmount() const {
    fileSystem->onTudUmount();
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void JSPico::onTudSuspend(bool remoteWakeUpEn) const {
    fileSystem->onTudSuspend(remoteWakeUpEn);
}

// Invoked when usb bus is resumed
void JSPico::onTudResume() const {
    fileSystem->onTudResume();
}