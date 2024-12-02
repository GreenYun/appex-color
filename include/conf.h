// Copyright (c) 2023-2024 GreenYun Organization
// SPDX-License-Identifier: MIT

#ifndef __APPEX_CONF_H__
#define __APPEX_CONF_H__

#define CONF_COLOR_DEFAULT 0xffffffff

#define DEFINE_STRING_ITEM(name) extern const char *conf_##name;
#define DEFINE_INT_ITEM(name)    extern int conf_##name;
#define DEFINE_COLOR_ITEM(name)  extern unsigned int conf_##name;

DEFINE_STRING_ITEM(main_background)
DEFINE_STRING_ITEM(scr_saver_item)
DEFINE_INT_ITEM(scr_saver_timeout)

DEFINE_STRING_ITEM(num_0)
DEFINE_STRING_ITEM(num_1)
DEFINE_STRING_ITEM(num_2)
DEFINE_STRING_ITEM(num_3)
DEFINE_STRING_ITEM(num_4)
DEFINE_STRING_ITEM(num_5)
DEFINE_STRING_ITEM(num_6)
DEFINE_STRING_ITEM(num_7)
DEFINE_STRING_ITEM(num_8)
DEFINE_STRING_ITEM(num_9)
DEFINE_STRING_ITEM(num_minus)
DEFINE_STRING_ITEM(num_plus)

DEFINE_INT_ITEM(rank_x)
DEFINE_INT_ITEM(rank_y)

DEFINE_COLOR_ITEM(rank_color)
DEFINE_COLOR_ITEM(normal_color)
DEFINE_COLOR_ITEM(negative_color)
DEFINE_COLOR_ITEM(positive_color)
DEFINE_COLOR_ITEM(check_color)
DEFINE_COLOR_ITEM(error_color)

void conf_init(void);

#endif // __APPEX_CONF_H__
