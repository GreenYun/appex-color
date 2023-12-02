// Copyright (c) 2023 GreenYun Organization
// SPDX-License-Identifier: MIT

#include <limits.h>
#include <time.h>

#include "btn.h"
#include "conf.h"
#include "graph.h"
#include "point.h"
#include "timer.h"

int main()
{
	conf_init();

	add_timeout_callback(graph_scr_saver);
	add_btn_click_callback(graph_toggle_mode);
	add_btn_long_press_callback(point_reinit);
	add_pt_chg_callback(graph_toggle_update);

	graph_init();
	point_init();

	for (;;)
		nanosleep(&(struct timespec) { .tv_sec = INT_MAX }, NULL);

	return 0;
}
