// Copyright (c) 2023-2024 GreenYun Organization
// SPDX-License-Identifier: MIT

#ifndef __APPEX_COMMON_H__
#define __APPEX_COMMON_H__

#define UNUSED(x) (void)(x)

#define timespec_s(n)  ((struct timespec) { .tv_sec = (n), .tv_nsec = 0 })
#define timespec_ms(n) ((struct timespec) { .tv_sec = 0, .tv_nsec = (n) * 1000000 })
#define timespec_us(n) ((struct timespec) { .tv_sec = 0, .tv_nsec = (n) * 1000 })

#endif // __APPEX_COMMON_H__
