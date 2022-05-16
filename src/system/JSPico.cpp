#include <pico/bootrom.h>
#include <pico/stdlib.h>

#include "JSPico.h"

#include "WebServerTask.h"
#include "FileSystem.h"
#include "FlashDisk.h"
#include "RamDisk.h"

// init device
void JSPico::init() {
    stdio_init_all();
    // create base file system
    fileSystem = new FileSystem();
    // init task manager
    taskManager = new TaskManager();

    // add flash device
//    fileSystem->addDevice(new FlashDisk());
    // add ram device
    FileSystem::addDevice(new RamDisk());

    // enable reboot by pressing boot sel button
    TaskManager::addTask(new BootSelTask(500));
    // blinking task used as state identification
    TaskManager::addTask(new BlinkTask(1000));
    // web server task
    TaskManager::addTask(new WebServerTask());
}

// Start main process
void JSPico::start() {
    while (taskManager->isNotEmpty()) {
        taskManager->processNextTask();
    }
    rebootRecovery();
}

// Reboot device
void JSPico::reboot() {
    // just clear all tasks, which continues to reboot
    taskManager->clear();
}

// Reboot device to recovery / flashing
void JSPico::rebootRecovery() {
    reset_usb_boot(0, 0);
}