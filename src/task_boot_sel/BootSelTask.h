#ifndef JSPICO_BOOTSELTASK_H
#define JSPICO_BOOTSELTASK_H

#include <cstdlib>
#include <cstdio>
#include <hardware/gpio.h>
#include <hardware/structs/ioqspi.h>
#include <hardware/structs/sio.h>
#include <pico/bootrom.h>
#include <pico/critical_section.h>
#include "pico/stdlib.h"

#include "Task.h"

const uint CS_PIN_INDEX = 1;

class BootSelTask : public Task {
private:
    uint button_delay;
    critical_section_t cs;
public:
    explicit BootSelTask(uint button_delay);

    ~BootSelTask();

    void init() override;

    void proc() override;

    void critical_sec_init();

    void critical_sec_done();

    void set_chip_select_override(const enum gpio_override override);

    bool __no_inline_not_in_flash_func(get_boot_sel_button)();

    void check_for_boot_sel_reset();
};

#endif //JSPICO_BOOTSELTASK_H
