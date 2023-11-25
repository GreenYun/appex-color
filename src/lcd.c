#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lcd.h"

#include "global_var.h"

void *lcd_thread(__attribute__((unused)) void *_arg)
{
	// fd is originally a global variable
	int fd;

	char str_buf[32];
	int count;
	ssize_t bytes_read, len;

	lcd_uptime = 0;

	memset(str_buf, 0, 32);
	fd = open("/etc/lcd_uptime", O_RDWR | O_CREAT, 0644);
	bytes_read = read(fd, str_buf, 30);
	printf("str_buf = %s\n\n", str_buf);

	if (bytes_read < 1) {
		sprintf(str_buf, "%d\n", lcd_uptime);
		len = strlen(str_buf);
		write(fd, str_buf, len);
	} else {
		lcd_uptime += atoi(str_buf);
	}

	close(fd);

	count = 0;
	while (1) {
		sleep(60);
		if (sleep_mode == 0) {
			pthread_mutex_lock(&mutex);
			if (reading != 0) {
				lcd_uptime += 1;
			}
			pthread_mutex_unlock(&mutex);
		}

		count += 1;
		printf("count = %d, lcd_uptime = %d\n", count, lcd_uptime);

		if (count > 30) {
			fd = open("/etc/lcd_uptime", O_RDWR | O_CREAT | O_TRUNC,
				  0644);

			pthread_mutex_lock(&mutex);
			sprintf(str_buf, "%d\n", lcd_uptime);
			pthread_mutex_unlock(&mutex);

			len = strlen(str_buf);
			write(fd, str_buf, len);

			close(fd);
			count = 0;
		}
	}
}
