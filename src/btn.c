// Copyright (c) 2023 GreenYun Organization
// SPDX-License-Identifier: MIT

#include "btn.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

void (*click_callback[3])(void);
uint8_t click_callback_count = 0;

void (*long_press_callback[3])(void);
uint8_t long_press_callback_count = 0;

void btn_init(void);
int  btn_thread(void *args);
void call(void (*callback[])(void), int count);

void add_btn_click_callback(void (*callback)(void))
{
	btn_init();

	if (click_callback_count < 3)
		click_callback[click_callback_count++] = callback;
}

void clear_btn_click_callback(void)
{
	click_callback_count = 0;
}

void add_btn_long_press_callback(void (*callback)(void))
{
	btn_init();

	if (long_press_callback_count < 3)
		long_press_callback[long_press_callback_count++] = callback;
}

void clear_btn_long_press_callback(void)
{
	long_press_callback_count = 0;
}

void btn_init(void)
{
	static bool thrd_started = false;

	if (!thrd_started) {
		thrd_t btn_thread_id;
		thrd_create(&btn_thread_id, btn_thread, NULL);
		thrd_started = true;
	}
}

int btn_thread(void *args)
{
	UNUSED(args);

	uint32_t    last_press = 0;
	time_t      last_press_time;
	bool        long_pressed = false;
	const char *gpio_path    = "/sys/class/gpio/gpio196/value";
	int         fd_gpio      = open(gpio_path, O_RDONLY);

	for (;;) {
		char val;
		lseek(fd_gpio, 0, SEEK_SET);
		read(fd_gpio, &val, 1);

		if (val == '0') {
			if (last_press == 0xffffffff) {
				time_t now = time(NULL);

				if (now - last_press_time >= 3 && !long_pressed) {
					call(long_press_callback, long_press_callback_count);
					long_pressed = true;
				}
			} else {
				last_press <<= 1;
				last_press |= 1;
				last_press_time = time(NULL);
			}
		} else if (val == '1') {
			if (last_press == 0xffffffff && !long_pressed) {
				call(click_callback, click_callback_count);
			}

			last_press   = 0;
			long_pressed = false;
		}

		thrd_sleep(&(struct timespec) { .tv_sec = 0, .tv_nsec = 1000000 }, NULL);
	}
}

void call(void (*callback[])(void), int count)
{
	for (int i = 0; i < count; i++) {
		callback[i]();
		nanosleep(&(struct timespec) { .tv_sec = 0, .tv_nsec = 2500000 }, NULL);
	}
}
