// Copyright (c) 2024 GreenYun Organization
// SPDX-License-Identifier: MIT

#include "gpio.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <linux/limits.h>

#include "common.h"

char *get_gpio_property(int gpio, const char *prop)
{
	char buf[PATH_MAX];
	int  n = snprintf(buf, PATH_MAX, "/sys/class/gpio/gpio%d/%s", gpio, prop);
	if (n < 0)
		return NULL;

	return strdup(buf);
}

int gpio_export(int gpio)
{
	FILE *fp = fopen("/sys/class/gpio/export", "w+");
	if (fp == NULL)
		return -1;

	fprintf(fp, "%d", gpio);
	fclose(fp);

	return 0;
}

int gpio_set_dir(int gpio, const char *dir)
{
	char *path = get_gpio_property(gpio, "direction");
	if (path == NULL)
		return -1;

	FILE *fp = fopen(path, "w+");
	free(path);

	if (fp == NULL)
		return -1;

	fprintf(fp, "%s", dir);
	fclose(fp);

	return 0;
}

char gpio_read(int gpio)
{
	char *path = get_gpio_property(gpio, "value");
	if (path == NULL)
		return -1;

	int fd = open(path, O_RDONLY);
	free(path);

	if (fd < 0)
		return -1;

	uint8_t val, last_press = 0xbb;
	for (;;) {
		lseek(fd, 0, SEEK_SET);
		read(fd, &val, 1);

		if (val == '1') {
			last_press <<= 1;
			last_press |= 1;
		} else if (val == '0') {
			last_press <<= 1;
			last_press &= 0xfe;
		} else {
			close(fd);
			return -1;
		}

		if (last_press == 0xff) {
			close(fd);
			return 1;
		} else if (last_press == 0x00) {
			close(fd);
			return 0;
		}

		nanosleep(&timespec_ms(5), NULL);
	}
}

int gpio_write(int gpio, const char val)
{
	char *path = get_gpio_property(gpio, "value");
	if (path == NULL)
		return -1;

	int fd = open(path, O_WRONLY);
	free(path);

	if (fd < 0)
		return -1;

	if (val == 0)
		write(fd, "0", 2);
	else
		write(fd, "1", 2);

	close(fd);

	return 0;
}
