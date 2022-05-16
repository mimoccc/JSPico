#ifndef JS_PICO_BLINK_TASK_ARGS_H
#define JS_PICO_BLINK_TASK_ARGS_H

#include <cassert>
#include <cinttypes>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Task.h"
#include "TaskManager.h"
#include "TaskArgs.h"
#include "BlinkTask.h"

class BlinkTaskArgs : public TaskArgs {
public:
    uint blinkDelayMs;

    BlinkTaskArgs(const char *sender, uint blink_delay_ms) : TaskArgs(sender, BLINK_TASK_NAME) {
        this->blinkDelayMs = blink_delay_ms;
    }

    ~BlinkTaskArgs() = default;
};

#endif //JS_PICO_BLINK_TASK_ARGS_H