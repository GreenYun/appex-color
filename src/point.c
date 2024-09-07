// Copyright (c) 2023 GreenYun Organization
// SPDX-License-Identifier: MIT

#include "point.h"

#include <fcntl.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

#include <modbus/modbus.h>

#include "common.h"
#include "conf.h"
#include "gpio.h"
#include "timer.h"

// Magic!
const char pos_map[4] = { 0, 3, 1, 2 };

int         pos;
pt_status_t points;

mtx_t       pt_mtx;
atomic_bool initialized;

modbus_t         *sync_mb;
modbus_mapping_t *sync_map;

void (*pt_chg_callback[3])(void);
int pt_chg_callback_count = 0;

int  chip_thread(void *arg);
void pos_init(void);
void sync_as_master(void);
void sync_as_slave(void);
int  sync_thread(void *arg);
void toggle_pt_update(void);

void add_pt_chg_callback(void (*callback)(void))
{
	if (pt_chg_callback_count < 3)
		pt_chg_callback[pt_chg_callback_count++] = callback;
}

void clear_pt_chg_callback(void)
{
	pt_chg_callback_count = 0;
}

void point_get(pt_status_t *restrict p)
{
	mtx_lock(&pt_mtx);
	*p = points;
	mtx_unlock(&pt_mtx);
}

void point_init(void)
{
	gpio_export(34);
	gpio_export(35);
	gpio_export(37);
	gpio_export(38);

	gpio_set_dir(34, "in");
	gpio_set_dir(35, "in");
	gpio_set_dir(37, "out");
	gpio_set_dir(38, "in");

	pos_init();

	gpio_write(37, 0);
	nanosleep(&timespec_ms(500), NULL);
	gpio_write(37, 1);

	gpio_write(38, 0);
	nanosleep(&timespec_ms(500), NULL);
	gpio_write(38, 1);

	for (int i = 0; i < 5; i++) {
		if (gpio_read(38) == 0)
			break;

		nanosleep(&timespec_ms(500), NULL);
	}

	if (gpio_read(38) != 0) {
		perror("STM32 failed");
		exit(1);
	}

	mtx_init(&pt_mtx, mtx_plain);
	atomic_store(&initialized, false);
	thrd_t thread_id;
	thrd_create(&thread_id, chip_thread, NULL);
	thrd_create(&thread_id, sync_thread, NULL);
}

void point_reinit(void)
{
	atomic_store(&initialized, false);
}

int chip_thread(void *arg)
{
	UNUSED(arg);

	modbus_t *mb = modbus_new_rtu("/dev/ttyS2", 115200, 'N', 8, 1);
	modbus_set_slave(mb, 16);
	modbus_connect(mb);
	modbus_set_response_timeout(mb, 0, 500000);
	modbus_flush(mb);

	for (;;) {
		thrd_sleep(&timespec_ms(500), NULL);

		uint16_t registers[6];
		if (modbus_read_registers(mb, 0, 6, registers) < 0) {
			modbus_flush(mb);
			continue;
		}

		bool update = false;

		mtx_lock(&pt_mtx);

		for (int i = 0; i < 5; i++) {
			if (registers[i] != points.chips[i]) {
				points.chips[i] = registers[i];
				update          = true;
			}
		}

		if (registers[5] != points.points[0]) {
			points.points[0] = registers[5];
			update           = true;
		}

		if (!atomic_exchange(&initialized, true)) {
			points.initial[0] = registers[5];
			update            = true;

			if (points.initial[0] == 0)
				timer_set(1);
			else
				timer_set(conf_scr_saver_timeout);
		}

		mtx_unlock(&pt_mtx);

		if (update) {
			toggle_pt_update();
		}
	}
}

void pos_init(void)
{
	char low  = gpio_read(34);
	char high = gpio_read(35);
	pos       = (high << 1) | low;
}

void sync_as_master(void)
{
	bool update = false;

	uint16_t read_buf[2];
	for (int i = 1; i <= 3; i++, thrd_sleep(&timespec_ms(150), NULL)) {
		modbus_set_slave(sync_mb, i);
		int ret = modbus_write_and_read_registers(sync_mb, 2, 8, points.points, 0, 2, read_buf);
		if (ret <= 0)
			continue;

		int off = pos_map[i];

		if (read_buf[0] != points.points[off] || read_buf[1] != points.initial[off]) {
			mtx_lock(&pt_mtx);
			points.points[off]  = read_buf[0];
			points.initial[off] = read_buf[1];
			mtx_unlock(&pt_mtx);

			update = true;
		}
	}

	if (update && points.initial[0] != 0) {
		toggle_pt_update();
	}

	thrd_sleep(&timespec_ms(200), NULL);
}

void sync_as_slave(void)
{
	bool    update = false;
	uint8_t buf[1024];

	modbus_set_slave(sync_mb, pos);
	int header_len = modbus_get_header_length(sync_mb);
	int rc         = modbus_receive(sync_mb, buf);
	if (rc <= 0)
		return;

	if (buf[header_len] == 0x17
		// Read 2 Registers
		&& *(uint32_t *)(buf + header_len + 1) == 0x02000000
		// Write 8 Registers
		&& *(uint32_t *)(buf + header_len + 5) == 0x08000200) {
		for (int i = 1; i <= 3; i++) {
			int off = (pos_map[pos] + i) % 4;

			uint16_t point   = (buf[header_len + 10 + off * 2] << 8) | buf[header_len + 11 + off * 2];
			uint16_t initial = (buf[header_len + 18 + off * 2] << 8) | buf[header_len + 19 + off * 2];

			if (point != points.points[i] || initial != points.initial[i]) {
				mtx_lock(&pt_mtx);
				points.points[i]  = point;
				points.initial[i] = initial;
				mtx_unlock(&pt_mtx);

				update = true;
			}
		}

		sync_map->tab_registers[0] = points.points[0];
		sync_map->tab_registers[1] = points.initial[0];
	}

	modbus_reply(sync_mb, buf, rc, sync_map);

	if (update && points.initial[0] != 0) {
		toggle_pt_update();
	}

	thrd_sleep(&timespec_ms(50), NULL);
}

int sync_thread(void *arg)
{
	UNUSED(arg);

	sync_mb = modbus_new_rtu("/dev/ttyS1", 115200, 'N', 8, 1);
	modbus_set_response_timeout(sync_mb, 0, 500000);
	modbus_connect(sync_mb);
	modbus_flush(sync_mb);
	sync_map = modbus_mapping_new(0, 0, 10, 0);

	for (;;) {
		if (pos == 0) {
			sync_as_master();
		} else {
			sync_as_slave();
		}
	}
}

void toggle_pt_update(void)
{
	for (int i = 0; i < pt_chg_callback_count; i++)
		pt_chg_callback[i]();
}
