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
# paths
set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/pico-sdk)
set(PICO_TINYUSB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinyusb)
set(BOARD_DIR ${CMAKE_CURRENT_LIST_DIR}/targets/${TARGET}/boards/${FAMILY})
set(SOURCES ${SOURCES} ${BOARD_DIR})
set(FATFS_DIR ${CMAKE_SOURCE_DIR}/src/fatfs)
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
        ${BOARD_DIR}/board.c
)
#
target_sources(${PROJECT} PUBLIC
        ${FATFS_DIR}/ff.c
        ${FATFS_DIR}/diskio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/msc/msc_disk.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/usb_descriptors.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/flash/flash.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)
#
target_include_directories(${PROJECT} PUBLIC
        ${TARGET_INC_DIR}
        ${BOARD_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/fatfs
        ${CMAKE_CURRENT_SOURCE_DIR}/src/msc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/usb
        ${PICO_SDK_PATH}/src/rp2_common/hardware_flash/include
        ${PICO_SDK_PATH}/src/rp2_common/hardware_gpio/include
)
#
pico_add_extra_outputs(${PROJECT_NAME})
#
target_compile_definitions(${PROJECT_NAME} PRIVATE
        PICO_ENTER_USB_BOOT_ON_EXIT=1
)
#
family_configure_device_example(${PROJECT})
###############################################################################