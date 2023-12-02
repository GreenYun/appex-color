// Copyright (c) 2023 GreenYun Organization
// SPDX-License-Identifier: MIT

#ifndef __APPEX_POINT_H__
#define __APPEX_POINT_H__

#include <stdint.h>

typedef struct pt_status {
	uint16_t chips[5];
	uint16_t points[4];
	uint16_t initial[4];
} pt_status_t;

void add_pt_chg_callback(void (*callback)(void));
void clear_pt_chg_callback(void);
void point_get(pt_status_t *restrict points);
void point_init(void);
void point_reinit(void);

#endif // __APPEX_POINT_H__
