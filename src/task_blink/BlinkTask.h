#ifndef JS_PICO_BLINK_TASK_H
#define JS_PICO_BLINK_TASK_H

#include "pico/stdlib.h"

#include "Task.h"

class BlinkTask : public Task {
private:
    uint blink_delay;

public:
    explicit BlinkTask(uint delay);

    ~BlinkTask();;

    void init() override;

    void proc() override;

    void ledOn();

    void ledOff();

    bool ledState();
};

#endif //JS_PICO_BLINK_TASK_H