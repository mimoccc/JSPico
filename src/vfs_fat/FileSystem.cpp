#include "FileSystem.h"
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/watchdog.h"
#include "hardware/structs/watchdog.h"

const char VID[] = "JSPico";
const char PID[] = "Mass Storage";
const char REV[] = "1.0";

void FileSystem::onTudResume() {
}

void FileSystem::onTudSuspend(bool b) {
}

void FileSystem::onTudMount() {
}

void FileSystem::onTudUmount() {
}

void FileSystem::addDevice(Device *device) {
    devices.push_back(device);
}

void FileSystem::getDeviceVendor(uint8_t devIndex, uint8_t *buffer) { // max 8 chars
    memcpy(buffer, VID, strlen(VID));
}

void FileSystem::getDeviceProduct(uint8_t devIndex, uint8_t *buffer) { // max 16 chars
    memcpy(buffer, PID, strlen(PID));
}

void FileSystem::getDeviceRevision(uint8_t devIndex, uint8_t *buffer) { // max 4 chars
    memcpy(buffer, REV, strlen(REV));
}

bool FileSystem::isDeviceReadOnly(uint8_t devIndex) {
    return devices[devIndex]->writeProtected;
}

bool FileSystem::isDeviceReady(uint8_t devIndex) {
    return devices[devIndex]->ready;
}
