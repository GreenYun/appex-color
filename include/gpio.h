// Copyright (c) 2024 GreenYun Organization
// SPDX-License-Identifier: MIT

#ifndef __APPEX_GPIO_H__
#define __APPEX_GPIO_H__

int gpio_export(int gpio);
int gpio_set_dir(int gpio, const char *dir);

char gpio_read(int gpio);
int  gpio_write(int gpio, const char val);

#endif // __APPEX_GPIO_H__
