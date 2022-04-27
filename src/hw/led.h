#ifndef JSPICO_BLINK_H
#define JSPICO_BLINK_H

enum {
    BLINK_NONE = 0,
    BLINK_STARTING = 500,
    BLINK_ERROR = 50,
//    BLINK_UNMOUNTED = 250,
//    BLINK_MOUNTED = 1000,
//    BLINK_SUSPENDED = 2500,
};

static uint32_t  blink_interval_ms = BLINK_NONE;

void led_task();

void set_led_blink_interval(uint32_t blink_interval_ms);

void led_on();

void led_off();

#endif //JSPICO_BLINK_H
