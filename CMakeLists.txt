cmake_minimum_required(VERSION 3.14)
include(FetchContent)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(CMAKE_STAGING_PREFIX ${STAGING_DIR})
set(CMAKE_SYSROOT ${CMAKE_STAGING_PREFIX}/host)
set(tools ${CMAKE_STAGING_PREFIX}/toolchain-arm_cortex-a8+vfpv3_gcc-7.3.0_musl_eabi)

set(CMAKE_C_COMPILER ${tools}/bin/arm-openwrt-linux-gcc)

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_DISABLE_FIND_PACKAGE_PkgConfig ON)
macro(pkg_check_modules)
endmacro()

project(app LANGUAGES C)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include ${CMAKE_SYSROOT}/include)
add_compile_definitions(USE_FBDEV)
add_compile_definitions(_GNU_SOURCE)

FILE(GLOB app_sources src/*.c src/**/*.c)
add_executable(app ${app_sources})

set_property(TARGET app PROPERTY C_STANDARD 11)
set_property(TARGET app PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
set_property(TARGET app PROPERTY LINK_FLAGS_RELEASE " -s ")
target_compile_options(app PUBLIC -Wall -Wextra -Wpedantic -O2)

set(LV_CONF_INCLUDE_SIMPLE)

FetchContent_Declare(lvgl 
                     GIT_REPOSITORY https://github.com/lvgl/lvgl.git
                     GIT_TAG release/v8.3)
FetchContent_MakeAvailable(lvgl)

FetchContent_Declare(lv_drivers
                     GIT_REPOSITORY https://github.com/lvgl/lv_drivers.git
                     GIT_TAG release/v8.3)
FetchContent_MakeAvailable(lv_drivers)

add_library(modbus STATIC IMPORTED)
set_target_properties(modbus PROPERTIES IMPORTED_LOCATION ${CMAKE_SYSROOT}/lib/libmodbus.a)

add_library(iniparser STATIC IMPORTED)
set_target_properties(iniparser PROPERTIES IMPORTED_LOCATION ${CMAKE_SYSROOT}/lib/libiniparser.a)

target_link_libraries(app PRIVATE lvgl::lvgl lvgl::drivers modbus iniparser)
