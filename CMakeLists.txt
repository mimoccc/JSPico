###############################################################################
# project cmake root config file
###############################################################################

cmake_minimum_required(VERSION 3.16)

# init git lis, just to be sure, all libs included
set(PICO_SDK_DIR ${CMAKE_SOURCE_DIR}/pico-sdk)
IF(NOT IS_DIRECTORY ${PICO_SDK_DIR})
    execute_process(
            COMMAND git clone https://github.com/raspberrypi/pico-sdk.git ${PICO_SDK_DIR} --recurse-submodules
    )
endif()

set(TINYUSB_DIR ${CMAKE_SOURCE_DIR}/tinyusb)
IF(NOT IS_DIRECTORY ${TINYUSB_DIR})
execute_process(
        COMMAND git clone https://github.com/hathach/tinyusb.git ${TINYUSB_DIR} --recurse-submodules
)
endif()

set(LWIP_DIR ${CMAKE_SOURCE_DIR}/lwip)
IF(NOT IS_DIRECTORY ${LWIP_DIR})
execute_process(
        COMMAND git clone https://git.savannah.nongnu.org/git/lwip.git ${LWIP_DIR} --recurse-submodules
)
endif()

# compiler
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PICO_ENABLE_CXX_EXCEPTIONS 1)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message(STATUS "CCache found. CCache will be used to cache project files.")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

# just build type message
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# project details
set(PROJECT_NAME "JSPico")
set(PROJECT_VERSION "1.0")
set(PROJECT_DESCRIPTION "JSPico - Use pico with javascript interpreted code on his own mass storage")

# pico sdk path, and lib paths
set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/pico-sdk)
set(PICO_TINYUSB_PATH ${CMAKE_SOURCE_DIR}/tinyusb)
set(LWIP_DIR ${CMAKE_SOURCE_DIR}/lwip)

# init pico sdk
include(pico-sdk/pico_sdk_init.cmake)

# configure project
project(
        ${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION ${PROJECT_DESCRIPTION}
        LANGUAGES CXX C ASM
)

# init pico sdk dependencies
pico_sdk_init()

# add executable
add_executable(${PROJECT_NAME} src/main.cpp)

# include libs
include(${CMAKE_SOURCE_DIR}/src/config/_lib.cmake)
include(${CMAKE_SOURCE_DIR}/src/modules/_lib.cmake)
include(${CMAKE_SOURCE_DIR}/src/system/_lib.cmake)
include(${CMAKE_SOURCE_DIR}/src/task/_lib.cmake)
include(${CMAKE_SOURCE_DIR}/src/task_blink/_lib.cmake)
include(${CMAKE_SOURCE_DIR}/src/task_boot_sel/_lib.cmake)
include(${CMAKE_SOURCE_DIR}/src/task_web_server/_lib.cmake)
include(${CMAKE_SOURCE_DIR}/src/utils/_lib.cmake)
include(${CMAKE_SOURCE_DIR}/src/vfs_fat/_lib.cmake)

# Link with the pico stdlib
target_link_libraries(${PROJECT_NAME} pico_stdlib pico_unique_id)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE PICO_ENTER_USB_BOOT_ON_EXIT=1)

#target_compile_options (${PROJECT_NAME} PUBLIC -fexceptions)
#target_compile_options (${PROJECT_NAME} PUBLIC -frtti)

# Enable USB serial
#pico_enable_stdio_semihosting(${PROJECT_NAME} 1)
#pico_enable_stdio_usb(${PROJECT_NAME} 1)
#pico_enable_stdio_uart(${PROJECT_NAME} 0)

###############################################################################
