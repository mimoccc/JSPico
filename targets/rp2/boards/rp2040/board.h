#ifndef __RP2_PICO_H
#define __RP2_PICO_H

// system
#define SYSTEM_ARCH     "cortex-m0-plus"
#define SYSTEM_PLATFORM "rp2"

// repl
#define REPL_BUFFER_SIZE  1024
#define REPL_HISTORY_SIZE 10

// Binary code, main fw
#define BINARY_SIZE (64 * 1024)
#define BINARY_MAX  BINARY_SIZE

// usb
#define DEV_USB_VID "JSPico      "
#define DEV_USB_PID "Mass Storage"
#define DEV_USB_REV "1.0 (C) 2022"

// flash rom
#define ROM_FLASH_OFFSET       BINARY_MAX
#define ROM_FLASH_DATA_SIZE    ((2*1024*1024)-BINARY_SIZE)
#define ROM_FLASH_SECTOR_SIZE  4096
#define ROM_FLASH_SECTOR_COUNT (FLASH_DATA_SIZE/FLASH_SECTOR_SIZE)
#define ROM_FLASH_PAGE_SIZE    256

#define VIRTUAL_DISC_SIZE    ROM_FLASH_DATA_SIZE
#define VIRTUAL_SEC_SIZE     512
#define VIRTUAL_SECTORS_CNT  (ROM_FLASH_DATA_SIZE / VIRTUAL_SEC_SIZE)

#define GPIO_COUNT 29
// #define ADC_NUM 3
#define PWM_NUM 27
#define I2C_NUM 2
#define SPI_NUM 2
#define UART_NUM 2
// #define LED_NUM 1
// #define BUTTON_NUM 0
#define PIO_NUM 2
#define PIO_SM_NUM 4

#define ADC_RESOLUTION_BIT 12
#define PWM_CLK_REF 1250
#define I2C_MAX_CLOCK 1000000
#define SCR_LOAD_GPIO 22  // GPIO 22

void target_board_init();

#endif /* __RP2_PICO_H */