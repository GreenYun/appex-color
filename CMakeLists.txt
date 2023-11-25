cmake_minimum_required(VERSION 3.14)

project(app LANGUAGES C)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)

include_directories(. include ${CMAKE_SYSROOT}/include)
add_compile_definitions(USE_FBDEV)

FILE(GLOB app_sources src/*.c)
add_executable(app ${app_sources})

set_property(TARGET app PROPERTY C_STANDARD 11)
target_compile_options(app PUBLIC -Wall -Wextra -Wpedantic -O2)

target_link_libraries(app PRIVATE modbus lvgl lv_drivers lvloadpng pthread)
