#ifndef JS_PICO_TASK_ARGS_H
#define JS_PICO_TASK_ARGS_H

#include <cassert>
#include <cinttypes>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

class TaskArgs {
public:
    const char *sender;
    const char *destination;

    explicit TaskArgs(const char *sender, const char* destination) {
        this->sender = sender;
        this->destination = destination;
    }

    ~TaskArgs();
};

#endif //JS_PICO_TASK_ARGS_H
