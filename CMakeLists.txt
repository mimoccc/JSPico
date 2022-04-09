cmake_minimum_required(VERSION 3.5)
set(PICO_TINYUSB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinyusb)
include(${PICO_TINYUSB_PATH}/hw/bsp/family_support.cmake)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)
family_get_project_name(PROJECT ${CMAKE_CURRENT_LIST_DIR})
project(${PROJECT})
family_initialize_project(${PROJECT} ${CMAKE_CURRENT_LIST_DIR})
add_executable(${PROJECT})
target_sources(${PROJECT} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/msc_disk.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usb_descriptors.c
)
target_include_directories(${PROJECT} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
pico_add_extra_outputs(${PROJECT_NAME})
#target_compile_definitions(${PROJECT_NAME} PRIVATE 
#    PICO_ENTER_USB_BOOT_ON_EXIT=1
#)
family_configure_device_example(${PROJECT})
