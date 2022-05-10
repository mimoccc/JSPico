set(LIB_NAME task_web_server)
set(LIB_PATH ${CMAKE_SOURCE_DIR}/src/task_web_server)

include(${LWIP_DIR}/src/Filelists.cmake)

add_library(${LIB_NAME} INTERFACE)

include_directories(${LIB_NAME} INTERFACE ${LIB_PATH})
include_directories(${LIB_NAME} INTERFACE "${LWIP_DIR}/src/include")
include_directories(${LIB_NAME} INTERFACE ${PICO_TINYUSB_PATH}/lib/networking/)

target_include_directories(${LIB_NAME} INTERFACE ${LIB_PATH})

target_sources(${LIB_NAME} INTERFACE ${PICO_TINYUSB_PATH}/lib/networking/dhserver.c)
target_sources(${LIB_NAME} INTERFACE ${PICO_TINYUSB_PATH}/lib/networking/rndis_reports.c)

target_sources(${PROJECT_NAME} INTERFACE ${LIB_PATH}/fsdata.c)
target_sources(${LIB_NAME} INTERFACE ${LIB_PATH}/tusb_lwip_glue.c)
target_sources(${LIB_NAME} INTERFACE ${LIB_PATH}/usb_descriptors.c)

target_sources(${LIB_NAME} INTERFACE ${LIB_PATH}/WebServerTask.cpp)

target_link_libraries(${PROJECT_NAME} ${LIB_NAME} tinyusb_device lwipcore lwipallapps)