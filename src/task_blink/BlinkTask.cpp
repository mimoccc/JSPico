#include "Task.h"
#include "TaskArgs.h"
#include "BlinkTask.h"
#include "BlinkTaskArgs.h"

BlinkTask::BlinkTask(uint blink_delay) : Task(BLINK_TASK_NAME, 1) {
    this->blink_delay = blink_delay;
}

BlinkTask::~BlinkTask() {
    Task::~Task();
}

void BlinkTask::init() {
    gpio_init(BOARD_LED);
    ledOn();
    Task::init();
}

void BlinkTask::proc() {
    if (ledState()) {
        ledOff();
    } else {
        ledOn();
    }
    delay(blink_delay);
}

void BlinkTask::ledOn() {
    gpio_set_dir(BOARD_LED, GPIO_OUT);
    gpio_put(BOARD_LED, true);
}

void BlinkTask::ledOff() {
    gpio_set_dir(BOARD_LED, GPIO_OUT);
    gpio_put(BOARD_LED, false);
}

bool BlinkTask::ledState() {
    gpio_set_dir(BOARD_LED, GPIO_IN);
    bool state = gpio_get(BOARD_LED);
    return state;
}

bool BlinkTask::setArgs(TaskArgs *args) {
    auto* a = (BlinkTaskArgs *) args;
    this->blink_delay = a->blinkDelayMs;
    return false;
}
