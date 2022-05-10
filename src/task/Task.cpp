#include "Task.h"

//#include <hardware/gpio.h>
//#include <hardware/regs/psm.h>
//#include <hardware/structs/ioqspi.h>
//#include <hardware/structs/sio.h>
//#include <hardware/sync.h>
#include <pico/bootrom.h>
//#include <pico/critical_section.h>
//#include <pico/stdlib.h>
//#include <cstdlib>
#include <cstdio>
//#include <cstring>
#include "bsp/board.h"

Task::Task(
        char const *task_name,
        uint8_t priority
) {
    this->initialized = false;
    this->task_name = task_name;
    this->priority = priority;
    this->suspended = false;
    this->schedule_ms = board_millis() + ((this->priority+1) * 10);
}

void Task::init() {
    initialized = true;
}

bool Task::isScheduledTime() const {
    return board_millis() > this->schedule_ms;
}

void Task::delay(uint delay) {
    this->schedule_ms += delay;
}

void Task::proc() {
}

Task::~Task() {
    initialized = false;
}

bool Task::isInitialized() const {
    return initialized;
}
