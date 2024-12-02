cmake_minimum_required(VERSION 3.24)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0177 NEW)

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

include(CheckIPOSupported)
message(CHECK_START "Checking Compiler Interprocedural Optimization Support")
check_ipo_supported(RESULT C_COMPILER_HAS_IPO_SUPPORT LANGUAGES C)

if(C_COMPILER_HAS_IPO_SUPPORT)
    message(CHECK_PASS "Success")
else()
    message(CHECK_FAIL "Failed")
endif()

include_directories(include ${CMAKE_SYSROOT}/include)
add_compile_definitions(USE_FBDEV)
add_compile_definitions(_GNU_SOURCE)

FILE(GLOB app_sources src/*.c src/**/*.c)
add_executable(app ${app_sources})

set_target_properties(app PROPERTIES
    C_STANDARD 11
    INTERPROCEDURAL_OPTIMIZATION ${C_COMPILER_HAS_IPO_SUPPORT}
)

target_compile_options(app PUBLIC -Wall -Wextra -Wpedantic)

include(CheckLinkerFlag)
check_linker_flag(C LINKER:-s C_LINKER_HAS_STRIP_ALL)
check_linker_flag(C LINKER:--as-needed C_LINKER_HAS_AS_NEEDED)

target_link_options(app PRIVATE
    $<$<AND:$<BOOL:${C_LINKER_HAS_STRIP_ALL}>,$<BOOL:$<CONFIG:Release>>>:LINKER:-s>
    $<$<BOOL:${C_LINKER_HAS_AS_NEEDED}>:LINKER:--as-needed>
)

include(FetchContent)

set(LV_CONF_INCLUDE_SIMPLE)
FetchContent_Declare(lvgl
    GIT_REPOSITORY https://github.com/lvgl/lvgl.git
    GIT_TAG release/v8.4
    PATCH_COMMAND patch -s -R -t -p1 -i ${CMAKE_SOURCE_DIR}/cmake/lvgl_png_color.patch &&
                  patch -s -N -f -p1 -i ${CMAKE_SOURCE_DIR}/cmake/lvgl_png_color.patch || true &&
                  patch -s -R -t -p1 -i ${CMAKE_SOURCE_DIR}/cmake/lvgl_png_color.patch &&
                  patch -s -N -f -p1 -i ${CMAKE_SOURCE_DIR}/cmake/lvgl_png_color.patch )
FetchContent_MakeAvailable(lvgl)

FetchContent_Declare(lv_drivers
    GIT_REPOSITORY https://github.com/lvgl/lv_drivers.git
    GIT_TAG release/v8.3)
FetchContent_MakeAvailable(lv_drivers)

set_target_properties(lvgl_demos lvgl_examples
    PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1
)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_TESTS OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_DOCS OFF)
FetchContent_Declare(iniparser
    GIT_REPOSITORY https://github.com/ndevilla/iniparser.git
    GIT_TAG main)
FetchContent_MakeAvailable(iniparser)

FetchContent_Declare(modbus
    GIT_REPOSITORY https://github.com/stephane/libmodbus.git
    GIT_TAG v3.1.11
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    OVERRIDE_FIND_PACKAGE
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cmake/modbus/CMakeLists.txt CMakeLists.txt &&
                  ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cmake/modbus/modbus-version.h.in src/modbus-version.h.in &&
                  ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cmake/modbus/config.h.in config.h.in
)
FetchContent_MakeAvailable(modbus)

target_link_libraries(app PRIVATE lvgl::lvgl lvgl::drivers iniparser-static modbus -static)
