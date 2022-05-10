set(LIB_NAME config)
set(LIB_PATH ${CMAKE_SOURCE_DIR}/src/config)
add_library(${LIB_NAME} INTERFACE)
#target_sources(${LIB_NAME} INTERFACE ${LIB_PATH}/js.c)
target_include_directories(${LIB_NAME} INTERFACE ${LIB_PATH})
target_link_libraries(${PROJECT_NAME} ${LIB_NAME})