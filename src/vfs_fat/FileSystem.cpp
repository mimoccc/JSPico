#include "FileSystem.h"
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/watchdog.h"
#include "hardware/structs/watchdog.h"

#include "FileSystem.h"
#include "JSPico.h"
#include "tusb.h"

const char VID[] = "JSPico";
const char PID[] = "Mass Storage";
const char REV[] = "1.0";

std::vector<Device *> FileSystem::devices = {};

FileSystem::FileSystem() {
}

FileSystem::~FileSystem() {
    devices.clear();
}

void FileSystem::onTudResume() {
}

void FileSystem::onTudSuspend(bool wakeEn) {
}

void FileSystem::onTudMount() {
}

void FileSystem::onTudUmount() {
}

void FileSystem::addDevice(Device *device) {
    device->init(devices.size());
    devices.push_back(device);
}

uint16_t FileSystem::deviceCount() {
    return devices.size();
}

void FileSystem::getDeviceVendor(uint8_t devIndex, uint8_t *buffer) {
    size_t len = strlen(VID);
    memcpy(buffer, VID, ((len > 8) ? 8 : len));
}

void FileSystem::getDeviceProduct(uint8_t devIndex, uint8_t *buffer) {
    size_t len = strlen(PID);
    memcpy(buffer, PID, ((len > 16) ? 16 : len));
}

void FileSystem::getDeviceRevision(uint8_t devIndex, uint8_t *buffer) {
    size_t len = strlen(REV);
    memcpy(buffer, REV, ((len > 4) ? 4 : len));
}

bool FileSystem::isDeviceReadOnly(uint8_t devIndex) {
    return devices[devIndex]->writeProtected;
}

bool FileSystem::isDeviceReady(uint8_t devIndex) {
    return  devices[devIndex]->ready();
}