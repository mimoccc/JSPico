//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "board.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

//-----------------------------------------------------------------------------

//void delay(int millis) {
//    vTaskDelay(millis / portTICK_PERIOD_MS);
//}

void board_delay_ms(uint32_t ms) {
    uint32_t start_ms = board_millis();
    uint32_t end_ms = 0;
    while (ms > end_ms) {
        end_ms = board_millis() - start_ms;
    }
}

void delay(int millis) {
    board_delay_ms(millis);
}

//-----------------------------------------------------------------------------