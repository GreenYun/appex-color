# Copyright (c) 2023-2024 GreenYun Organization
# SPDX-License-Identifier: MIT

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(OPENWRT_STAGING_DIR $ENV{STAGING_DIR})

set(CMAKE_SYSROOT ${OPENWRT_STAGING_DIR}/host)

set(OPENWRT_TOOLCHAIN ${OPENWRT_STAGING_DIR}/toolchain-arm_cortex-a8+vfpv3_gcc-7.3.0_musl_eabi)
set(CMAKE_C_COMPILER ${OPENWRT_TOOLCHAIN}/bin/arm-openwrt-linux-muslgnueabi-gcc)
set(CMAKE_CXX_COMPILER ${OPENWRT_TOOLCHAIN}/bin/arm-openwrt-linux-muslgnueabi-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_DISABLE_FIND_PACKAGE_PkgConfig ON)
macro(pkg_check_modules)
endmacro()
