// Copyright (c) 2023 GreenYun Organization
// SPDX-License-Identifier: MIT

#include "conf.h"

#include <stdio.h>
#include <string.h>

#include <iniparser.h>

#include "common.h"

#define DECLARE_STRING_ITEM(name)          \
	const char *conf_##name        = NULL; \
	const char *conf_##name##_NAME = "config:" #name;
#define DECLARE_INT_ITEM(name)          \
	int         conf_##name        = 0; \
	const char *conf_##name##_NAME = "config:" #name;
#define DECLARE_COLOR_ITEM(name)                          \
	unsigned int conf_##name        = CONF_COLOR_DEFAULT; \
	const char  *conf_##name##_NAME = "config:" #name;

#define LOAD_STRING_ITEM(name)                                        \
	conf_##name = iniparser_getstring(ini, conf_##name##_NAME, NULL); \
	if (conf_##name != NULL)                                          \
		conf_##name = strdup(conf_##name);

#define LOAD_INT_ITEM(name, default)   conf_##name = iniparser_getint(ini, conf_##name##_NAME, default);
#define LOAD_COLOR_ITEM(name, default) conf_##name = iniparser_getlongint(ini, conf_##name##_NAME, default);

DECLARE_STRING_ITEM(main_background)
DECLARE_STRING_ITEM(scr_saver_item)
DECLARE_INT_ITEM(scr_saver_timeout)

DECLARE_STRING_ITEM(num_0)
DECLARE_STRING_ITEM(num_1)
DECLARE_STRING_ITEM(num_2)
DECLARE_STRING_ITEM(num_3)
DECLARE_STRING_ITEM(num_4)
DECLARE_STRING_ITEM(num_5)
DECLARE_STRING_ITEM(num_6)
DECLARE_STRING_ITEM(num_7)
DECLARE_STRING_ITEM(num_8)
DECLARE_STRING_ITEM(num_9)
DECLARE_STRING_ITEM(num_minus)
DECLARE_STRING_ITEM(num_plus)

DECLARE_INT_ITEM(rank_x)
DECLARE_INT_ITEM(rank_y)

DECLARE_COLOR_ITEM(rank_color)
DECLARE_COLOR_ITEM(normal_color)
DECLARE_COLOR_ITEM(negative_color)
DECLARE_COLOR_ITEM(positive_color)
DECLARE_COLOR_ITEM(check_color)
DECLARE_COLOR_ITEM(error_color)

const char *conf_file_search[] = {
	"./appex.conf",
	"./appex.ini",
	"~/.appex.conf",
	"/etc/appex.conf",
};

int null_error_callback(const char *fmt, ...)
{
	UNUSED(fmt);
	return 0;
}

void conf_init(void)
{
	dictionary *ini;

	iniparser_set_error_callback(null_error_callback);

	for (size_t i = 0; i < sizeof conf_file_search / sizeof conf_file_search[0]; i++) {
		ini = iniparser_load(conf_file_search[i]);
		if (ini != NULL) {
			break;
		}
	}

	LOAD_STRING_ITEM(main_background)
	LOAD_STRING_ITEM(scr_saver_item)
	LOAD_INT_ITEM(scr_saver_timeout, 30)

	LOAD_STRING_ITEM(num_0)
	LOAD_STRING_ITEM(num_1)
	LOAD_STRING_ITEM(num_2)
	LOAD_STRING_ITEM(num_3)
	LOAD_STRING_ITEM(num_4)
	LOAD_STRING_ITEM(num_5)
	LOAD_STRING_ITEM(num_6)
	LOAD_STRING_ITEM(num_7)
	LOAD_STRING_ITEM(num_8)
	LOAD_STRING_ITEM(num_9)
	LOAD_STRING_ITEM(num_minus)
	LOAD_STRING_ITEM(num_plus)

	LOAD_INT_ITEM(rank_x, 60)
	LOAD_INT_ITEM(rank_y, -45)

	LOAD_COLOR_ITEM(rank_color, 0xfdfdfd)
	LOAD_COLOR_ITEM(normal_color, 0xfdfdfd)
	LOAD_COLOR_ITEM(negative_color, 0x1e10cd)
	LOAD_COLOR_ITEM(positive_color, 0xcf0a20)
	LOAD_COLOR_ITEM(check_color, 0x21dbcf)
	LOAD_COLOR_ITEM(error_color, 0x1f5ac0)

	if (ini != NULL)
		iniparser_freedict(ini);
}
