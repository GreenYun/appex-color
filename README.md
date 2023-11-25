# app

Reversed edition with some fixes.

## Library Dependencies

- LVGL (drivers) 5.3
- libmodbus 3.1.4

## Build Requisites

- Any C compiler (supported C11 standard) and linker.
- CMake 3.14 or later.
- Git

## Build Instructions

Change working directory to the root of this repository, then run the following
commands:

```sh
git clone -b release/v5 https://github.com/lvgl/lvgl
git clone -b release/v5 https://github.com/lvgl/lv_drivers
git clone https://github.com/lvgl/lv_lib_png
sed 's/if\s0/if 1/1' lvgl/lv_conf_templ.h >lv_conf.h
sed 's/if\s0/if 1/1' lv_drivers/lv_drv_conf_templ.h >lv_drv_conf.h
mkdir build && cd build
cmake .. # Specify your favorite build system and other options here.
cmake --build .
```
