#include "Task.h"
#include "BootSelTask.h"

static char const *BOOT_SEL_TASK_NAME = "boot_sel_task";

BootSelTask::BootSelTask(uint button_delay) : Task(BOOT_SEL_TASK_NAME, 1) {
    this->button_delay = button_delay;
}

BootSelTask::~BootSelTask() {
    Task::~Task();
}

void BootSelTask::init() {
    Task::init();
}

void BootSelTask::proc() {
    check_for_boot_sel_reset();
}

void BootSelTask::critical_sec_init() {
    critical_section_init(&cs);
    critical_section_enter_blocking(&cs);
}

void BootSelTask::critical_sec_done() {
    critical_section_exit(&cs);
    critical_section_deinit(&cs);
}

void BootSelTask::set_chip_select_override(const enum gpio_override override) {
    hw_write_masked(&ioqspi_hw->io[CS_PIN_INDEX].ctrl,
                    override << IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_LSB,
                    IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_BITS);
}

bool BootSelTask::get_boot_sel_button() {
    critical_sec_init();
    set_chip_select_override(GPIO_OVERRIDE_LOW);
    for (unsigned int i = 0; i < 1000; ++i) {
        __asm("nop");
    };
    bool button_state = !(sio_hw->gpio_hi_in & (1u << CS_PIN_INDEX));
    set_chip_select_override(GPIO_OVERRIDE_NORMAL);
    critical_sec_done();
    return button_state;
}

void BootSelTask::check_for_boot_sel_reset() {
    if (get_boot_sel_button()) {
        sleep_ms(500);
        if (get_boot_sel_button()) {
            reset_usb_boot(0, 0);
        }
    }
}