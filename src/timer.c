// Copyright (c) 2023 GreenYun Organization
// SPDX-License-Identifier: MIT

#include "timer.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <threads.h>
#include <unistd.h>

#include "common.h"

int        timeout;
atomic_int time_idle;
void (*timeout_callback[3])(void);
int timeout_callback_count = 0;

int timeout_thread(void *args);

void add_timeout_callback(void (*callback)(void))
{
	if (timeout_callback_count < 3)
		timeout_callback[timeout_callback_count++] = callback;
}

void clear_timeout_callback(void)
{
	timeout_callback_count = 0;
}

void timer_reset(void)
{
	atomic_store(&time_idle, 0);
}

void timer_set(int minutes)
{
	static bool thrd_started = false;

	timeout = minutes;
	atomic_store(&time_idle, 0);

	if (!thrd_started) {
		thrd_t thread_id;
		thrd_create(&thread_id, timeout_thread, NULL);
		thrd_started = true;
	}
}

int timeout_thread(void *arg)
{
	UNUSED(arg);

	for (;;) {
		thrd_sleep(&timespec_s(60), NULL);

		int time = atomic_fetch_add(&time_idle, 1);

		if (++time == timeout) {
			for (int i = 0; i < timeout_callback_count; i++) {
				timeout_callback[i]();
				nanosleep(&timespec_us(2500), NULL);
			}
		} else if (time > timeout)
			atomic_store(&time_idle, timeout + 1);
	}
}
