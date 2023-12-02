// Copyright (c) 2023 GreenYun Organization
// SPDX-License-Identifier: MIT

#ifndef __APPEX_BTN_H__
#define __APPEX_BTN_H__

void add_btn_click_callback(void (*callback)(void));
void clear_btn_click_callback(void);

void add_btn_long_press_callback(void (*callback)(void));
void clear_btn_long_press_callback(void);

#endif // __APPEX_BTN_H__
