#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "gpio.h"

int gpio_export(unsigned int gpio)
{
	char s[64];
	size_t n;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
	} else {
		n = snprintf(s, 0x40, "%d", gpio);
		write(fd, s, n);

		close(fd);
		fd = 0;
	}

	return fd;
}

int gpio_set_dir(unsigned int gpio, int dir)
{
	char path[64];
	int fd;

	snprintf(path, 64, "/sys/class/gpio/gpio%d/direction", gpio);

	fd = open(path, 1);
	if (fd < 0) {
		perror("gpio set direction failed");
	} else {
		if (dir == GPIO_DIR_OUT) {
			write(fd, "in", 3);
		} else {
			write(fd, "out", 4);
		}

		close(fd);
		fd = 0;
	}

	return fd;
}

int gpio_get_value(unsigned int gpio, int *val)
{
	char reading;
	char acStack_4c[64];
	int fd;

	snprintf(acStack_4c, 0x40, "/sys/class/gpio/gpio%d/value", gpio);
	fd = open(acStack_4c, 0);

	if (fd < 0) {
		perror("gpio/get-value");
	} else {
		read(fd, &reading, 1);
		if (reading == '0') {
			*val = 0;
		} else {
			*val = 1;
		}

		close(fd);
		fd = 0;
	}

	return fd;
}

int gpio_set_value(unsigned int gpio, int val)
{
	char path[64];
	int fd;

	snprintf(path, 0x40, "/sys/class/gpio/gpio%d/value", gpio);

	fd = open(path, 1);
	if (fd < 0) {
		perror("gpio/set-value");
	} else {
		if (val == 0)
			write(fd, "0", 2);
		else
			write(fd, "1", 2);

		close(fd);
		fd = 0;
	}

	return fd;
}
