###############################################################################
# Project cmake file
###############################################################################
# default target: rp2
if (NOT TARGET)
    set(TARGET "rp2")
endif ()
# default family: rp2040
if (NOT FAMILY)
    set(FAMILY "rp2040")
endif ()
###############################################################################
cmake_minimum_required(VERSION 3.5)
# project name
set(PROJECT "JSPico")
#
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
# paths
set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/libs/pico-sdk)
set(PICO_TINYUSB_PATH ${CMAKE_SOURCE_DIR}/libs/tinyusb)
set(BOARD_DIR ${CMAKE_CURRENT_LIST_DIR}/targets/${TARGET}/boards/${FAMILY})
set(FATFS_DIR ${CMAKE_SOURCE_DIR}/src/fatfs)
set (MODULES_DIR ${CMAKE_SOURCE_DIR}/src/modules)
set(SOURCES ${SOURCES} ${BOARD_DIR} ${FATFS_DIR} ${MODULES_DIR})
# cmake include files
include(${PICO_TINYUSB_PATH}/hw/bsp/family_support.cmake)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
#
family_get_project_name(PROJECT ${CMAKE_CURRENT_LIST_DIR})
# main project
project(${PROJECT})
#
family_initialize_project(${PROJECT} ${CMAKE_CURRENT_LIST_DIR})
#
add_executable(
        ${PROJECT}
#        ${CMAKE_SOURCE_DIR}/src/js/js.c
        ${BOARD_DIR}/board.c
)
#
target_sources(${PROJECT} PUBLIC
        ${CMAKE_SOURCE_DIR}/src/utils/base64.c
        ${BOARD_DIR}/board.c
        ${FATFS_DIR}/ff.c
        ${FATFS_DIR}/diskio.c
        ${MODULES_DIR}/md_board/md_board.c
        ${MODULES_DIR}/md_global/md_global.c
        ${MODULES_DIR}/md_console/md_console.c
        ${MODULES_DIR}/md_gpio/md_gpio.c
        ${MODULES_DIR}/md_fs/md_fs.c
        ${CMAKE_SOURCE_DIR}/src/js/js.c
        ${CMAKE_SOURCE_DIR}/src/log/log.c
        ${CMAKE_SOURCE_DIR}/src/hw/led.c
        ${CMAKE_SOURCE_DIR}/src/boot/boot.c
        ${CMAKE_SOURCE_DIR}/src/msc/msc_disk.c
        ${CMAKE_SOURCE_DIR}/src/usb/usb_descriptors.c
        ${CMAKE_SOURCE_DIR}/src/flash/flash_disk.c
        ${CMAKE_SOURCE_DIR}/src/fs_data.c
        ${CMAKE_SOURCE_DIR}/src/tasks/task_main.c
        ${CMAKE_SOURCE_DIR}/src/tasks/task_js.c
        ${CMAKE_SOURCE_DIR}/src/main.c
        )
#
target_include_directories(${PROJECT} PUBLIC
        ${CMAKE_SOURCE_DIR}/src/log
        ${PICO_SDK_PATH}/src/rp2_common/pico_multicore/include
        ${TARGET_INC_DIR}
        ${BOARD_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/utils
        ${CMAKE_SOURCE_DIR}/src/js
        ${CMAKE_SOURCE_DIR}/src/hw
        ${CMAKE_SOURCE_DIR}/src/boot
        ${CMAKE_SOURCE_DIR}/src/flash
        ${CMAKE_SOURCE_DIR}/src/fatfs
        ${CMAKE_SOURCE_DIR}/src/msc
        ${CMAKE_SOURCE_DIR}/src/usb
        ${PICO_SDK_PATH}/src/rp2_common/hardware_flash/include
        ${PICO_SDK_PATH}/src/rp2_common/hardware_gpio/include

        )
#
# add_subdirectory(src/freertos)
#
pico_add_extra_outputs(${PROJECT_NAME})
#
target_compile_definitions(${PROJECT_NAME} PRIVATE
        PICO_ENTER_USB_BOOT_ON_EXIT=1
        )
#
family_configure_device_example(${PROJECT})
###############################################################################