// Copyright (c) 2023 GreenYun Organization
// SPDX-License-Identifier: MIT

#include "graph.h"

#include <fcntl.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

#include <display/fbdev.h>
#include <lvgl.h>

#include "common.h"
#include "conf.h"
#include "point.h"
#include "timer.h"

LV_IMG_DECLARE(NUM_0)
LV_IMG_DECLARE(NUM_1)
LV_IMG_DECLARE(NUM_2)
LV_IMG_DECLARE(NUM_3)
LV_IMG_DECLARE(NUM_4)
LV_IMG_DECLARE(NUM_5)
LV_IMG_DECLARE(NUM_6)
LV_IMG_DECLARE(NUM_7)
LV_IMG_DECLARE(NUM_8)
LV_IMG_DECLARE(NUM_9)
LV_IMG_DECLARE(NUM_MINUS)

const void *nums[12]
	= { &NUM_0, &NUM_1, &NUM_2, &NUM_3, &NUM_4, &NUM_5, &NUM_6, &NUM_7, &NUM_8, &NUM_9, &NUM_MINUS, NULL };

lv_obj_t  *scr_main, *scr_saver;
lv_style_t main_style;

lv_obj_t  *check_led, *check_digits[7], *digits[4][8];
lv_style_t check_style, digit_style[4], rank_style;

mtx_t       task_mtx;
atomic_bool main_update;
atomic_bool main_diff_mode;
atomic_bool scr_saver_running;

void backlight_init(void);
void backlight_off(void);
int  get_rank(int pos, uint16_t *points);
void main_init(void);
void main_show_points(void);
void main_show_diff(void);
int  main_thread(void *arg);
void nums_init(void);
int  scr_saver_thread(void *args);
int  task_thread(void *args);
int  timer_thread(void *args);
void translate_digits(int point, bool diff, lv_obj_t *(d[]));

void graph_init(void)
{
	static lv_color_t *buf1 = NULL;
	static lv_color_t *buf2 = NULL;

	backlight_init();
	lv_init();
	fbdev_init();

	uint32_t width, height, dpi;
	fbdev_get_sizes(&width, &height, &dpi);

	uint32_t buf_size = width * height / 8;

	buf1 = malloc(buf_size * sizeof(lv_color_t));
	buf2 = malloc(buf_size * sizeof(lv_color_t));

	if (buf1 == NULL) {
		perror("malloc failed");
		exit(1);
	}

	static lv_disp_draw_buf_t disp_buf;
	lv_disp_draw_buf_init(&disp_buf, buf1, buf2, buf_size);

	static lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.draw_buf  = &disp_buf;
	disp_drv.flush_cb  = fbdev_flush;
	disp_drv.hor_res   = width;
	disp_drv.ver_res   = height;
	disp_drv.dpi       = dpi;
	disp_drv.rotated   = LV_DISP_ROT_270;
	disp_drv.sw_rotate = 1;
	lv_disp_drv_register(&disp_drv);

	scr_main = lv_scr_act();

	mtx_init(&task_mtx, mtx_plain);
	atomic_store(&main_update, false);
	atomic_store(&main_diff_mode, false);

	thrd_t thread_id;
	thrd_create(&thread_id, task_thread, NULL);
	thrd_create(&thread_id, timer_thread, NULL);
	thrd_create(&thread_id, main_thread, NULL);
}

void graph_scr_saver(void)
{
	static bool thrd_started = false;

	if (!thrd_started) {
		scr_saver = lv_obj_create(NULL);

		thrd_t scr_saver_thread_id;
		thrd_create(&scr_saver_thread_id, scr_saver_thread, NULL);
		thrd_started = true;
	}

	bool already_running = atomic_exchange(&scr_saver_running, true);

	if (!already_running) {
		mtx_lock(&task_mtx);
		lv_scr_load(scr_saver);
		mtx_unlock(&task_mtx);

		clear_timeout_callback();
		add_timeout_callback(backlight_off);
		timer_reset();
	}
}

void graph_toggle_mode(void)
{
	atomic_store(&main_diff_mode, !atomic_load(&main_diff_mode));
	graph_toggle_update();
}

void graph_toggle_update(void)
{
	backlight_init();
	atomic_store(&main_update, true);

	bool saver_running = atomic_exchange(&scr_saver_running, false);
	if (saver_running) {
		mtx_lock(&task_mtx);
		lv_scr_load(scr_main);
		mtx_unlock(&task_mtx);
	}

	clear_timeout_callback();
	add_timeout_callback(graph_scr_saver);
	timer_reset();
}

void backlight_init(void)
{
	const char *path       = "/sys/class/backlight/backlight/brightness";
	int         fd         = open(path, O_WRONLY);
	char        brightness = '1';
	write(fd, &brightness, 1);
	close(fd);
}

void backlight_off(void)
{
	const char *path       = "/sys/class/backlight/backlight/brightness";
	int         fd         = open(path, O_WRONLY);
	char        brightness = '0';
	write(fd, &brightness, 1);
	close(fd);
}

int get_rank(int pos, uint16_t *points)
{
	int point = points[pos];
	int rank  = 1;
	for (int i = 0; i < 4; i++) {
		if (points[i] > point)
			rank++;
	}

	return rank;
}

void main_init(void)
{
	nums_init();

	mtx_lock(&task_mtx);

	lv_style_init(&main_style);
	if (conf_main_background != NULL)
		lv_style_set_bg_img_src(&main_style, conf_main_background);
	else
		lv_style_set_bg_color(&main_style, lv_color_hex(0x03f0000));
	lv_obj_add_style(scr_main, &main_style, 0);

	check_led = lv_obj_create(scr_main);
	lv_obj_set_style_border_color(check_led, lv_color_hex(conf_error_color), 0);
	lv_obj_set_style_bg_color(check_led, lv_color_hex(conf_check_color), 0);
	lv_obj_set_width(check_led, 24);
	lv_obj_set_height(check_led, 24);
	lv_obj_set_style_radius(check_led, LV_RADIUS_CIRCLE, 0);

	lv_style_init(&check_style);
	lv_style_set_img_recolor_opa(&check_style, LV_OPA_COVER);
	lv_style_set_img_recolor(&check_style, lv_color_hex(conf_check_color));

	for (int i = 0; i < 7; i++) {
		check_digits[i] = lv_img_create(scr_main);
		lv_img_set_src(check_digits[i], nums[8]);
		lv_img_set_zoom(check_digits[i], 128);
		lv_obj_set_width(check_digits[i], LV_SIZE_CONTENT);
		lv_obj_set_height(check_digits[i], LV_SIZE_CONTENT);
		lv_img_set_size_mode(check_digits[i], LV_IMG_SIZE_MODE_REAL);
		lv_obj_add_style(check_digits[i], &check_style, 0);
		lv_obj_set_pos(check_digits[i], (6 - i) * 24, 0);
	}

	lv_style_init(&rank_style);
	lv_style_set_img_recolor_opa(&rank_style, LV_OPA_COVER);
	lv_style_set_img_recolor(&rank_style, lv_color_hex(conf_rank_color));

	for (int i = 0; i < 4; i++) {
		lv_style_init(digit_style + i);
		lv_style_set_img_recolor_opa(digit_style + i, LV_OPA_COVER);
		lv_style_set_img_recolor(digit_style + i, lv_color_hex(conf_normal_color));

		for (int j = 0; j < 8; j++) {
			digits[i][j] = lv_img_create(scr_main);
			lv_img_set_src(digits[i][j], nums[8]);
			lv_img_set_zoom(digits[i][j], j == 7 ? 128 : j < 2 ? 192 : i == 0 ? 320 : 256);
			lv_obj_set_width(digits[i][j], LV_SIZE_CONTENT);
			lv_obj_set_height(digits[i][j], LV_SIZE_CONTENT);
			lv_img_set_size_mode(digits[i][j], LV_IMG_SIZE_MODE_REAL);
			lv_obj_add_style(digits[i][j], j == 7 ? &rank_style : digit_style + i, 0);

			int x = j == 7 ? conf_rank_x : j < 2 ? (1 - j) * 42 : -(j - 1) * (i == 0 ? 64 : 55);
			int y = j == 7 ? conf_rank_y : j < 2 ? 18 : i == 0 ? -22 : 0;
			switch (i) {
			case 0:
				lv_obj_set_pos(digits[i][j], 526 + x, 378 + y);
				break;
			case 1:
				lv_obj_set_pos(digits[i][j], 748 + x, 200 + y);
				break;
			case 2:
				lv_obj_set_pos(digits[i][j], 548 + x, 50 + y);
				break;
			case 3:
				lv_obj_set_pos(digits[i][j], 278 + x, 200 + y);
				break;
			default:
				break;
			}
		}
	}

	mtx_unlock(&task_mtx);
}

void main_show_points(void)
{
	pt_status_t points;
	point_get(&points);

	bool check = points.points[0] + points.points[1] + points.points[2] + points.points[3]
				 == points.initial[0] + points.initial[1] + points.initial[2] + points.initial[3];

	mtx_lock(&task_mtx);

	for (int i = 0; i < 7; i++)
		lv_obj_add_flag(check_digits[i], LV_OBJ_FLAG_HIDDEN);

	lv_obj_clear_flag(check_led, LV_OBJ_FLAG_HIDDEN);
	if (check)
		lv_obj_set_style_bg_opa(check_led, LV_OPA_COVER, 0);
	else
		lv_obj_set_style_bg_opa(check_led, LV_OPA_TRANSP, 0);

	for (int i = 0; i < 4; i++) {
		if (points.initial[i] == 0) {
			for (int j = 0; j < 8; j++)
				lv_obj_add_flag(digits[i][j], LV_OBJ_FLAG_HIDDEN);
			continue;
		}

		int rank = get_rank(i, points.points);
		lv_img_set_src(digits[i][7], nums[rank]);
		lv_obj_clear_flag(digits[i][7], LV_OBJ_FLAG_HIDDEN);

		translate_digits(points.points[i], false, digits[i]);
		lv_style_set_img_recolor(digit_style + i, lv_color_hex(0xfdfdfd));
	}

	mtx_unlock(&task_mtx);
}

void main_show_diff(void)
{
	pt_status_t points;
	point_get(&points);

	uint16_t total = points.points[0] + points.points[1] + points.points[2] + points.points[3];
	bool     check = total == points.initial[0] + points.initial[1] + points.initial[2] + points.initial[3];

	mtx_lock(&task_mtx);

	lv_obj_add_flag(check_led, LV_OBJ_FLAG_HIDDEN);
	translate_digits(total, false, check_digits);

	if (check)
		lv_style_set_img_recolor(&check_style, lv_color_hex(conf_check_color));
	else
		lv_style_set_img_recolor(&check_style, lv_color_hex(conf_error_color));

	int rank = get_rank(0, points.points);
	lv_img_set_src(digits[0][7], nums[rank]);
	translate_digits(points.points[0], false, digits[0]);

	for (int i = 1; i < 4; i++) {
		if (points.initial[i] == 0) {
			for (int j = 0; j < 8; j++)
				lv_obj_add_flag(digits[i][j], LV_OBJ_FLAG_HIDDEN);
			continue;
		}

		int rank = get_rank(i, points.points);
		lv_img_set_src(digits[i][7], nums[rank]);
		lv_obj_clear_flag(digits[i][7], LV_OBJ_FLAG_HIDDEN);

		int diff = (int)points.points[i] - (int)points.points[0];
		translate_digits(diff, true, digits[i]);

		uint32_t color = conf_normal_color;
		if (diff > 0)
			color = conf_positive_color;
		else if (diff < 0)
			color = conf_negative_color;

		lv_style_set_img_recolor(digit_style + i, lv_color_hex(color));
	}

	mtx_unlock(&task_mtx);
}

int main_thread(void *arg)
{
	UNUSED(arg);

	main_init();

	for (;;) {
		thrd_sleep(&(struct timespec) { .tv_sec = 0, .tv_nsec = 100000000 }, NULL);

		if (!atomic_exchange(&main_update, false))
			continue;

		if (atomic_load(&main_diff_mode))
			main_show_diff();
		else
			main_show_points();
	}
}

void nums_init(void)
{
#define INIT_NUM(n)                 \
	do {                            \
		if (conf_num_##n != NULL)   \
			nums[n] = conf_num_##n; \
	} while (0)

	INIT_NUM(0);
	INIT_NUM(1);
	INIT_NUM(2);
	INIT_NUM(3);
	INIT_NUM(4);
	INIT_NUM(5);
	INIT_NUM(6);
	INIT_NUM(7);
	INIT_NUM(8);
	INIT_NUM(9);

#undef INIT_NUM

	if (conf_num_minus != NULL)
		nums[10] = conf_num_minus;
	if (conf_num_plus != NULL)
		nums[11] = conf_num_plus;
}

int scr_saver_thread(void *arg)
{
	UNUSED(arg);

	uint32_t palette[]     = { 0xbe00ff, 0x00feff, 0x0026ff, 0xff8300, 0xff2600, 0xff008b, 0xfffa01 };
	uint32_t palette_index = time(NULL) % 7;

	mtx_lock(&task_mtx);

	lv_style_t img_style;
	lv_style_init(&img_style);
	lv_style_set_img_recolor_opa(&img_style, LV_OPA_COVER);
	lv_style_set_img_recolor(&img_style, lv_color_hex(palette[palette_index]));

	// LV_IMG_DECLARE(DVD_VIDEO_LOGO);
	lv_obj_t *img_dvd = lv_img_create(scr_saver);
	if (conf_scr_saver_item != NULL)
		lv_img_set_src(img_dvd, conf_scr_saver_item);
	// else
	//	lv_img_set_src(img_dvd, &DVD_VIDEO_LOGO);

	lv_obj_add_style(img_dvd, &img_style, 0);

	mtx_unlock(&task_mtx);

	uint32_t width, height;
	fbdev_get_sizes(&height, &width, NULL);

	struct timespec ts;

	timespec_get(&ts, TIME_UTC);
	int32_t pos_x = ts.tv_nsec % 120;

	timespec_get(&ts, TIME_UTC);
	int32_t pos_y = ts.tv_nsec % 120;

	int32_t dir_x = 3, dir_y = 3;
	bool    dir_changed = false;

	for (uint32_t t = 0;; t++) {
		thrd_sleep(&(struct timespec) { .tv_sec = 0, .tv_nsec = 5000000 }, NULL);

		if (!atomic_load(&scr_saver_running)) {
			thrd_sleep(&(struct timespec) { .tv_sec = 1 }, NULL);
			continue;
		}

		if (t % 6)
			continue;

		if (pos_x >= (int32_t)(width - DVD_VIDEO_LOGO.header.w)) {
			dir_x       = -3;
			dir_changed = true;
		} else if (pos_x <= 0) {
			dir_x       = 3;
			dir_changed = true;
		}
		pos_x += dir_x;

		if (pos_y >= (int32_t)(height - DVD_VIDEO_LOGO.header.h)) {
			dir_y       = -3;
			dir_changed = true;
		} else if (pos_y <= 0) {
			dir_y       = 3;
			dir_changed = true;
		}
		pos_y += dir_y;

		mtx_lock(&task_mtx);

		lv_obj_set_pos(img_dvd, pos_x, pos_y);

		if (dir_changed) {
			timespec_get(&ts, TIME_UTC);
			palette_index = (palette_index + ts.tv_nsec % 6 + 1) % 7;
			lv_style_set_img_recolor(&img_style, lv_color_hex(palette[palette_index]));
		}

		mtx_unlock(&task_mtx);

		dir_changed = 0;
	}
}

int task_thread(void *arg)
{
	UNUSED(arg);

	for (;;) {
		mtx_lock(&task_mtx);
		lv_task_handler();
		mtx_unlock(&task_mtx);

		thrd_sleep(&(struct timespec) { .tv_sec = 0, .tv_nsec = 5000000 }, NULL);
	}
}

int timer_thread(void *arg)
{
	UNUSED(arg);

	for (;;) {
		lv_tick_inc(5);
		thrd_sleep(&(struct timespec) { .tv_sec = 0, .tv_nsec = 5000000 }, NULL);
	}
}

void translate_digits(int point, bool diff, lv_obj_t *(d[]))
{
	bool negative = false, positive = diff && nums[11] != NULL;
	int  current = 0;

	if (point < 0) {
		point    = -point;
		negative = true;
	}

	if (point >= 1000) {
		if (negative) {
			lv_img_set_src(d[6], nums[10]);
			lv_obj_clear_flag(d[6], LV_OBJ_FLAG_HIDDEN);
			negative = false;
		} else if (positive) {
			lv_img_set_src(d[6], nums[11]);
			lv_obj_clear_flag(d[6], LV_OBJ_FLAG_HIDDEN);

		} else {
			lv_obj_add_flag(d[6], LV_OBJ_FLAG_HIDDEN);
		}

		positive = false;

		current = point / 1000 % 10;
		lv_img_set_src(d[5], nums[current]);
		lv_obj_clear_flag(d[5], LV_OBJ_FLAG_HIDDEN);
	} else {
		lv_obj_add_flag(d[6], LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(d[5], LV_OBJ_FLAG_HIDDEN);
	}

	if (point >= 100) {
		if (negative) {
			lv_img_set_src(d[5], nums[10]);
			lv_obj_clear_flag(d[5], LV_OBJ_FLAG_HIDDEN);
			negative = false;
		} else if (positive) {
			lv_img_set_src(d[5], nums[11]);
			lv_obj_clear_flag(d[5], LV_OBJ_FLAG_HIDDEN);
		}

		positive = false;

		current = point / 100 % 10;
		lv_img_set_src(d[4], nums[current]);
		lv_obj_clear_flag(d[4], LV_OBJ_FLAG_HIDDEN);
	} else {
		lv_obj_add_flag(d[4], LV_OBJ_FLAG_HIDDEN);
	}

	if (point >= 10) {
		if (negative) {
			lv_img_set_src(d[4], nums[10]);
			lv_obj_clear_flag(d[4], LV_OBJ_FLAG_HIDDEN);
			negative = false;
		} else if (positive) {
			lv_img_set_src(d[4], nums[11]);
			lv_obj_clear_flag(d[4], LV_OBJ_FLAG_HIDDEN);
		}

		positive = false;

		current = point / 10 % 10;
		lv_img_set_src(d[3], nums[current]);
		lv_obj_clear_flag(d[3], LV_OBJ_FLAG_HIDDEN);
	} else {
		lv_obj_add_flag(d[3], LV_OBJ_FLAG_HIDDEN);
	}

	if (point > 0) {
		if (negative) {
			lv_img_set_src(d[3], nums[10]);
			lv_obj_clear_flag(d[3], LV_OBJ_FLAG_HIDDEN);
		} else if (positive) {
			lv_img_set_src(d[3], nums[11]);
			lv_obj_clear_flag(d[3], LV_OBJ_FLAG_HIDDEN);
		}

		current = point % 10;
		lv_img_set_src(d[2], nums[current]);
		lv_obj_clear_flag(d[2], LV_OBJ_FLAG_HIDDEN);
	} else {
		lv_img_set_src(d[2], nums[0]);
	}

	lv_obj_clear_flag(d[2], LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(d[1], nums[0]);
	lv_obj_clear_flag(d[1], LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(d[0], nums[0]);
	lv_obj_clear_flag(d[0], LV_OBJ_FLAG_HIDDEN);
}
