#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <modbus/modbus.h>

#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"

#include "global_var.h"

#include "gpio.h"
#include "lcd.h"
#include "load.h"
#include "comm.h"
#include "read.h"
#include "show.h"

unsigned short south;
unsigned short east;
unsigned short west;
unsigned short north;

unsigned short dummy;

unsigned short default_south;
unsigned short default_east;
unsigned short default_west;
unsigned short default_north;

int lcd_uptime;
int lcd_status;
int header_length;

short action_flag;
unsigned char show_state;
unsigned char start_flag;
unsigned char sync_flag;
unsigned char check_flag;
unsigned char sleep_mode;
unsigned char reading;
unsigned char special_start;
unsigned char start_bit;

pthread_mutex_t mutex;

time_t action_time;
time_t now_time;

modbus_t *local_mb;
modbus_t *remote_mb;

modbus_mapping_t *mb_mapping;

int get_startbit(void);

int main()
{
	int upgrading = 0;
	int val = 1;
	int debounce = 0;

	unsigned char buf[256];

	uint16_t reg_data[8];

	pthread_t lcd_thread_id;
	pthread_t read_thread_id;
	pthread_t modbus_thread_id;

	lv_disp_drv_t disp_drv;

	gpio_export(0x25);
	usleep(50000);
	gpio_set_dir(0x25, GPIO_DIR_IN);
	usleep(50000);
	gpio_set_value(0x25, 0);
	usleep(50000);
	gpio_set_value(0x25, 1);

	system("echo 4 > /sys/class/backlight/backlight/brightness");
	action_flag = 1;

	lv_init();
	start_flag = 0;

	if (pthread_mutex_init(&mutex, NULL) != 0)
		return -1;

	pthread_create(&lcd_thread_id, NULL, lcd_thread, NULL);

	get_startbit();
	printf("start_bit = %d\n", start_bit);

	action_time = time(NULL);
	sleep_mode = 0;

	local_mb = modbus_new_rtu("/dev/ttyS2", 115200, 'N', 8, 1);
	remote_mb = modbus_new_rtu("/dev/ttyS1", 115200, 'N', 8, 1);
	modbus_set_slave(local_mb, 0x10);
	modbus_connect(local_mb);
	modbus_set_response_timeout(local_mb, 0, 500000);

	fbdev_init();

	reading = 0;

	lv_disp_drv_init(&disp_drv);
	disp_drv.disp_flush = fbdev_flush;
	lv_disp_drv_register(&disp_drv);

	load_png_resource();
	showpoint_create_obj();

	mb_mapping = modbus_mapping_new(0, 0, 8, 0);
	modbus_connect(remote_mb);
	modbus_flush(remote_mb);

	load_startup();

	// Periodically run this to update LGVL
	for (int i = 0; i < 30; i++) {
		lv_tick_inc(5);
		lv_task_handler();
		usleep(5000);
	}

	for (int initialized = 0;; initialized = 1) {
		if (initialized != 0) {
			usleep(50000);
			gpio_set_value(0x25, 0);
			usleep(50000);
			gpio_set_value(0x25, 1);
			usleep(50000);
		}

		int failure_count = 0;
		default_south = 0;

		modbus_flush(local_mb);

		usleep(200000);

		// Try 10 times to ensure we get the maximum point reading (stability)
		for (int i = 0; i < 10; i++) {
			for (int retry_count = 0; retry_count <= 300;
			     retry_count++) {
				gpio_get_value(0x26, &val);
				usleep(200000);
				if (val != 1)
					break;
			}

			int ret =
				modbus_read_registers(local_mb, 0, 8, reg_data);
			if (ret < 0) {
				modbus_flush(local_mb);
				modbus_close(local_mb);
				modbus_free(local_mb);
				usleep(200000);

				local_mb = modbus_new_rtu("/dev/ttyS2", 115200,
							  'N', 8, 1);
				modbus_set_slave(local_mb, 0x10);
				modbus_connect(local_mb);
				modbus_set_response_timeout(remote_mb, 0,
							    500000);
				failure_count += 1;
			} else if (default_south < reg_data[5]) {
				default_south = reg_data[5];
				if (reg_data[5] != 0)
					break;
			}

			printf("%d ret = %d\n", 663, ret);
		}

		if (failure_count <= 8)
			break;

		if (initialized != 0) {
			show_failed(1);
			do {
				lv_tick_inc(5);
				lv_task_handler();
				usleep(5000);
			} while (true);
		}
	}

	show_failed(0);

	for (int i = 0; i < 30; i++) {
		lv_tick_inc(5);
		lv_task_handler();
		usleep(5000);
	}

	printf("default_south = %d\n", default_south);

	switch (start_bit) {
	case 0:
		// I'm west
		modbus_set_slave(remote_mb, 2);
		mb_mapping->tab_registers[2] = default_south;
		break;
	case 1:
		// I'm north
		modbus_set_slave(remote_mb, 3);
		mb_mapping->tab_registers[3] = default_south;
		break;
	case 2:
		// I'm east
		modbus_set_slave(remote_mb, 1);
		mb_mapping->tab_registers[1] = default_south;
	case 3:
		// I'm south (the master)
		break;
	default:
		printf("unknown start_bit, error\n");
		return -1;
	}

	if (default_south == 0) {
		// No player
		reading = 0;
		system("echo 0 >/sys/class/backlight/backlight/brightness");
	} else {
		if (pthread_create(&read_thread_id, NULL, read_thread, NULL) !=
		    0) {
			printf("start thread error\n");
			return -1;
		}
		reading = 1;
	}

	default_east = 0;
	default_west = 0;
	default_north = 0;

	if (start_bit == 3) {
		// I'm south (master)

		// Get players
		int i;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				int retry_count = 0;
				modbus_set_slave(remote_mb, j + 1);
				while (1) {
					int rc = modbus_read_registers(
						remote_mb, 0, 8, reg_data);
					retry_count += 1;
					if (rc > -1 || retry_count > 2)
						break;
					printf("timeout! retry %d %d...\n", j,
					       retry_count);
					sleep(1);
				}

				if (j == 1 && default_north <= reg_data[2])
					default_north = reg_data[2];
				else if (j == 2 && default_east < reg_data[3])
					default_east = reg_data[3];
				else if (j == 0 && default_west <= reg_data[1])
					default_west = reg_data[1];
			}

			usleep(100000);
		}

		// Test how many players
		i = 4;
		if (default_east == 0)
			i -= 1;
		if (default_north == 0)
			i -= 1;
		if (default_west == 0)
			i -= 1;
		if (default_south == 0)
			i -= 1;

		if (i == 0) {
			// No players
			i = 1;
			special_start = 1;
		}

		printf("i = %d \n", i);
		special_start = 0;

		special_start = (default_south + default_east + default_west +
				 default_north) %
				i;

		if (pthread_create(&modbus_thread_id, NULL,
				   modbus_master_thread, NULL) != 0) {
			printf("start thread error\n");
			return -1;
		}
	} else {
		header_length = modbus_get_header_length(remote_mb);

		int rc;
		while (true) {
			rc = modbus_receive(remote_mb, buf);
			unsigned len = ((unsigned)buf[header_length + 3] << 8) +
				       buf[header_length + 4];
			if (rc > 0) {
				printf("rc = %d, len = %d\n", rc, len);

				if ((buf[header_length] == '\x03') &&
				    (*(uint32_t *)(buf + header_length + 1) ==
				     0x08000000))
					break;
			}

			modbus_flush(remote_mb);
		}

		usleep(20000);
		modbus_reply(remote_mb, buf, rc, mb_mapping);

		if (pthread_create(&modbus_thread_id, NULL, modbus_slave_thread,
				   NULL) != 0) {
			printf("start thread error\n");
			return -1;
		}
	}

	sleep(2);
	hide_startup();
	load_num(east, west, south, north, 0, 0, show_state);

	for (int i = 0; i < 30; i++) {
		lv_tick_inc(5);
		lv_task_handler();
		usleep(5000);
	}

	start_flag = 1;
	lcd_status = 0;
	show_state = 0;

	for (int i = 0;;) {
		lv_tick_inc(1);
		lv_task_handler();
		usleep(1000);

		gpio_get_value(0xc4, &val);
		if (val == 0) {
			usleep(10000);
			gpio_get_value(0xc4, &val);
			if (val == 0) {
				debounce = 0;
				do {
					usleep(5000);
					gpio_get_value(0xc4, &val);
					debounce += 1;
				} while (val == 0);

				show_state = !show_state;
				pthread_mutex_lock(&mutex);
				action_time = time(NULL);
				printf("action time change!\n");

				if (debounce <= 6000 || lcd_status == 0) {
					if (debounce > 2000) {
						printf("lcd_status = %d\n",
						       lcd_status);
						lcd_status = !lcd_status;
						load_num(0, 0, lcd_uptime / 60,
							 0, 0, 0, 0);
					}
				} else {
					lcd_uptime = 0;
					load_num(0, 0, 0, 0, 0, 0, 0);
					int fd =
						open("/etc/lcd_uptime",
						     O_RDWR | O_CREAT | O_TRUNC,
						     0644);
					char s[32];
					sprintf(s, "%d", lcd_uptime);
					size_t __n = strlen(s);
					write(fd, s, __n);
					close(fd);
				}
				pthread_mutex_unlock(&mutex);
			}
		}

		val = 1;
		gpio_get_value(0xc2, &val);
		if (val == 0) {
			usleep(10000);
			gpio_get_value(0xc2, &val);
			if (val == 0) {
				debounce = 0;

				do {
					usleep(5000);
					gpio_get_value(0xc2, &val);
					debounce = debounce + 1;
				} while (val == 0);

				if (debounce > 400) {
					upgrading = 1;

					system("echo 6 >/sys/class/backlight/backlight/brightness");
					printf("upgrading!\n");

					if (upgrading != 0) {
						system("echo 2 >/sys/class/backlight/backlight/brightness");

						for (int i = 0; i < 30;
						     i = i + 1) {
							lv_tick_inc(5);
							lv_task_handler();
							usleep(5000);
						}

						show_upgrading();

						for (int i = 0; i < 30;
						     i = i + 1) {
							lv_tick_inc(5);
							lv_task_handler();
							usleep(5000);
						}

						int local_48 = system(
							"/bin/upgrade.sh");

						if ((((int)local_48 >> 8 &
						      0xffU) == 0) &&
						    ((local_48 & 0x7f) == 0)) {
							show_upgraded(0);
						} else {
							show_upgraded(1);
						}

						sleep(3);

						for (int i = 0; i < 30;
						     i = i + 1) {
							lv_tick_inc(5);
							lv_task_handler();
							usleep(5000);
						}

						sleep(2);
						system("reboot");
					}

					modbus_mapping_free(mb_mapping);
					modbus_close(local_mb);
					modbus_close(remote_mb);
					modbus_free(local_mb);
					modbus_free(remote_mb);
					return 0;
				}
			}
		}

		i += 1;
		if (i == 6) {
			i = 0;
			if (pthread_mutex_lock(&mutex) == 0) {
				if (lcd_status == 0)
					load_num(east, west, south, north,
						 check_flag, 0, show_state);

				pthread_mutex_unlock(&mutex);
			}
		}
	}
}

int get_startbit(void)
{
	int high, low;

	// DIP switch
	gpio_get_value(0x22, &low);
	gpio_get_value(0x23, &high);

	start_bit = (low & 1) + (char)((high & 1) << 1);

	return printf("gpio34 = %d \ngpio35 = %d\n", low, high);
}
