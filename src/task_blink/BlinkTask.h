#ifndef JS_PICO_BLINK_TASK_H
#define JS_PICO_BLINK_TASK_H

#include "pico/stdlib.h"

#include "Task.h"
#include "TaskArgs.h"

static const uint BOARD_LED = 25;
static char const *BLINK_TASK_NAME = "blink_task";

class BlinkTask : public Task {
private:
    uint blink_delay;

public:
    explicit BlinkTask(uint delay);

    ~BlinkTask();;

    void init() override;

    void proc() override;

    static void ledOn();

    static void ledOff();

    static bool ledState();

    bool setArgs(TaskArgs *args);
};

#endif //JS_PICO_BLINK_TASK_H