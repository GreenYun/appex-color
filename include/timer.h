// Copyright (c) 2023-2024 GreenYun Organization
// SPDX-License-Identifier: MIT

#ifndef __APPEX_TIMER_H__
#define __APPEX_TIMER_H__

void add_timeout_callback(void (*callback)(void));
void clear_timeout_callback(void);
void timer_reset(void);
void timer_set(int minutes);

#endif // __APPEX_TIMER_H__
