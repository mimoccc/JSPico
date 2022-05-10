#ifndef JS_PICO_TASK_H
#define JS_PICO_TASK_H

#include <cassert>
#include <cinttypes>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

class Task {
public:
    const char *task_name;      // task name
    uint32_t schedule_ms;       // next task cycle function run schedule
    uint8_t priority;           // task priority 1-max 255 - min
    bool initialized;           // task is initialized
    bool suspended;             // task is halted, sleeping

    Task(char const *task_name, uint8_t priority);

    ~Task();

    bool isScheduledTime() const;

    void delay(uint delay);

    virtual void init();

    virtual void proc();

    bool isInitialized() const;
};

#endif //JS_PICO_TASK_H