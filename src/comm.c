#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "comm.h"

#include <modbus/modbus.h>

#include "global_var.h"

unsigned long remote_action_time;
unsigned long remote_now_time;

void *modbus_master_thread(__attribute__((unused)) void *_arg)
{
	uint16_t registers[10];

	while (1) {
		action_flag = 0;

		modbus_set_slave(remote_mb, 1);
		if (modbus_read_registers(remote_mb, 0, 8, registers) > 0) {
			pthread_mutex_lock(&mutex);
			west = registers[1];
			pthread_mutex_unlock(&mutex);

			sync_flag = 1;

			remote_action_time = ((uint32_t)registers[4] << 16) |
					     registers[5];
			remote_now_time = ((uint32_t)registers[6] << 16) |
					  registers[7];

			if ((remote_now_time - remote_action_time) < 1800) {
				pthread_mutex_lock(&mutex);
				action_flag = 1;
				pthread_mutex_unlock(&mutex);
			}
		}

		usleep(100000);

		modbus_set_slave(remote_mb, 2);
		if (modbus_read_registers(remote_mb, 0, 8, registers) > 0) {
			pthread_mutex_lock(&mutex);
			north = registers[2];
			pthread_mutex_unlock(&mutex);
			sync_flag = 1;

			remote_action_time = ((uint32_t)registers[4] << 16) |
					     registers[5];
			remote_now_time = ((uint32_t)registers[6] << 16) |
					  registers[7];

			if (remote_now_time - remote_action_time < 1800) {
				pthread_mutex_lock(&mutex);
				action_flag = 1;
				pthread_mutex_unlock(&mutex);
			}
		}

		usleep(100000);

		modbus_set_slave(remote_mb, 3);
		if (modbus_read_registers(remote_mb, 0, 8, registers) > 0) {
			pthread_mutex_lock(&mutex);
			east = registers[3];
			pthread_mutex_unlock(&mutex);
			sync_flag = 1;

			remote_action_time = ((uint32_t)registers[4] << 16) |
					     registers[5];
			remote_now_time = ((uint32_t)registers[6] << 16) |
					  registers[7];

			if (remote_now_time - remote_action_time < 1800) {
				pthread_mutex_lock(&mutex);
				action_flag = 1;
				pthread_mutex_unlock(&mutex);
			}
		}

		usleep(100000);
		pthread_mutex_lock(&mutex);
		now_time = time(NULL);
		if (now_time - action_time < 1800)
			action_flag = 1;

		if (action_flag == 0) {
			system("echo 0 >/sys/class/backlight/backlight/brightness");
			sleep_mode = 1;
		} else if (reading != 0) {
			system("echo 6 >/sys/class/backlight/backlight/brightness");
			sleep_mode = 0;
		}

		registers[0] = south;
		pthread_mutex_unlock(&mutex);
		registers[1] = west;
		registers[2] = north;
		registers[3] = east;
		uint16_t check = south + west + north + east ==
				 default_east + default_north + default_south +
					 default_west;
		registers[4] = check + (special_start << 1);
		registers[5] = default_south;
		registers[6] = default_west;
		registers[7] = default_north;
		registers[8] = default_east;

		pthread_mutex_lock(&mutex);
		check_flag = registers[4];
		pthread_mutex_unlock(&mutex);

		printf("send A = %d, B = %d, C = %d, D = %d, flag = %d\n",
		       registers[0], registers[1], registers[2], registers[3],
		       registers[4]);

		registers[9] = action_flag;

		modbus_set_slave(remote_mb, 1);
		modbus_write_registers(remote_mb, 0, 10,
				       (uint16_t *)&registers);
		usleep(100000);

		modbus_set_slave(remote_mb, 2);
		modbus_write_registers(remote_mb, 0, 10,
				       (uint16_t *)&registers);
		usleep(100000);

		modbus_set_slave(remote_mb, 3);
		modbus_write_registers(remote_mb, 0, 10,
				       (uint16_t *)&registers);
		usleep(100000);
	}
}

#define COMBINE_HL(high, low) \
	((uint16_t)(((uint16_t)(high) << 8) + (uint16_t)(low)))

void *modbus_slave_thread(__attribute__((unused)) void *_arg)
{
	unsigned char buf[256];
	int rc;
	int i;

	while (1) {
		rc = modbus_receive(remote_mb, buf);
		if (rc > 0) {
			printf("\n\n");
			for (i = 0; i < rc; i++)
				printf("%02x ", buf[i]);
			printf("\n\n");

			now_time = time(NULL);

			// Read Holding Registers Request
			if (buf[header_length] == 0x03 &&
			    // Starting: 0000h, Quantity: 0008h
			    // [1]: 0, [2]: 0, [3]: 0, [4]: 8
			    // hope I did not mess byte order
			    *(uint32_t *)(buf + header_length + 1) ==
				    0x08000000) {
				usleep(20000);

				mb_mapping->tab_registers[4] =
					(uint16_t)(action_time >> 16);
				mb_mapping->tab_registers[5] =
					(uint16_t)action_time;
				mb_mapping->tab_registers[6] =
					(uint16_t)(now_time >> 16);
				mb_mapping->tab_registers[7] =
					(uint16_t)now_time;

				modbus_reply(remote_mb, buf, rc, mb_mapping);
			}

			// Write Multiple Registers Request
			else if (buf[header_length] == 0x10 &&
				 // Starting: 0000h, Quantity: 000Ah
				 // [1]: 0, [2]: 0, [3]: 0, [4]: 10
				 *(uint32_t *)(buf + header_length + 1) ==
					 0x0a000000) {
				pthread_mutex_lock(&mutex);
				check_flag = buf[header_length + 15];

				switch (start_bit) {
				case 1:
					// I'm north
					west = COMBINE_HL(
						buf[header_length + 6],
						buf[header_length + 7]);
					north = COMBINE_HL(
						buf[header_length + 8],
						buf[header_length + 9]);
					east = COMBINE_HL(
						buf[header_length + 10],
						buf[header_length + 11]);
					// dummy = COMBINE_HL(buf[header_length + 12],
					//	buf[header_length + 13]);

					default_west = COMBINE_HL(
						buf[header_length + 16],
						buf[header_length + 17]);
					default_north = COMBINE_HL(
						buf[header_length + 18],
						buf[header_length + 19]);
					default_east = COMBINE_HL(
						buf[header_length + 20],
						buf[header_length + 21]);
					dummy = COMBINE_HL(
						buf[header_length + 22],
						buf[header_length + 23]);

					printf("receive A = %d, B = %d, C = %d, D = %d, flag = %d\n",
					       west, north, east, dummy,
					       check_flag);

					break;

				case 2:
					// I'm east
					east = COMBINE_HL(
						buf[header_length + 6],
						buf[header_length + 7]);
					// dummy = COMBINE_HL(buf[header_length + 8],
					//	buf[header_length + 9]);
					west = COMBINE_HL(
						buf[header_length + 10],
						buf[header_length + 11]);
					north = COMBINE_HL(
						buf[header_length + 12],
						buf[header_length + 13]);

					default_east = COMBINE_HL(
						buf[header_length + 16],
						buf[header_length + 17]);
					dummy = COMBINE_HL(
						buf[header_length + 18],
						buf[header_length + 19]);
					default_west = COMBINE_HL(
						buf[header_length + 20],
						buf[header_length + 21]);
					default_north = COMBINE_HL(
						buf[header_length + 22],
						buf[header_length + 23]);

					printf("receive A = %d, B = %d, C = %d, D = %d, flag = %d\n",
					       east, dummy, west, north,
					       check_flag);

					break;

				case 0:
					// I'm west
					north = COMBINE_HL(
						buf[header_length + 6],
						buf[header_length + 7]);
					east = COMBINE_HL(
						buf[header_length + 8],
						buf[header_length + 9]);
					// dummy = COMBINE_HL(buf[header_length + 10],
					//	buf[header_length + 11]);
					west = COMBINE_HL(
						buf[header_length + 12],
						buf[header_length + 13]);

					default_north = COMBINE_HL(
						buf[header_length + 16],
						buf[header_length + 17]);
					default_east = COMBINE_HL(
						buf[header_length + 18],
						buf[header_length + 19]);
					dummy = COMBINE_HL(
						buf[header_length + 20],
						buf[header_length + 21]);
					default_west = COMBINE_HL(
						buf[header_length + 22],
						buf[header_length + 23]);

					printf("receive A = %d, B = %d, C = %d, D = %d, flag = %d\n",
					       north, east, dummy, west,
					       check_flag);

				default:
					break;
				}

				action_flag =
					COMBINE_HL(buf[header_length + 24],
						   buf[header_length + 25]);

				if (action_flag == 0) {
					system("echo 0 >/sys/class/backlight/backlight/brightness");
					sleep_mode = 1;
				} else if (reading != '\0') {
					system("echo 6 >/sys/class/backlight/backlight/brightness");
					sleep_mode = 0;
				}

				pthread_mutex_unlock(&mutex);
				sync_flag = 1;
				usleep(20000);

				modbus_reply(remote_mb, buf, rc, mb_mapping);
			}
		}
		modbus_flush(remote_mb);
	}
}
