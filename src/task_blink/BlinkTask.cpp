#include "Task.h"
#include "BlinkTask.h"

static const uint BOARD_LED = 25;
static char const *BLINK_TASK_NAME = "blink_task";

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