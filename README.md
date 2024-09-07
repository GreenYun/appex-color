appex-color
===========

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


If it works, it works.

Build Instructions
------------------

Requires:

- CMake
- Ninja
- OpenWRT SDK
- patch

```sh
export MY_BUILD
mkdir -p $MY_BUILD
cd $MY_BUILD
curl -L https://archive.openwrt.org/releases/18.06.4/targets/sunxi/cortexa8/openwrt-sdk-18.06.4-sunxi-cortexa8_gcc-7.3.0_musl_eabi.Linux-x86_64.tar.xz | tar -xJ
export STAGING_DIR=$PWD/openwrt-sdk-18.06.4-sunxi-cortexa8_gcc-7.3.0_musl_eabi.Linux-x86_64/staging_dir
git clone https://github.com/GreenYun/appex-color.git
cd appex-color
cmake -B build -G Ninja -D CMAKE_BUILD_TYPE=Release -D CMAKE_TOOLCHAIN_FILE=toolchain.cmake
ninja -C build
```
