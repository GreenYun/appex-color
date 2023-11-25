#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "read.h"

#include <modbus/modbus.h>

#include "global_var.h"
#include "gpio.h"

time_t last_time;
time_t last_last_time;
unsigned short last_point;
unsigned short last_last_point;

void *read_thread(__attribute__((unused)) void *_arg)
{
	int local_9 = 0;

	last_time = time(NULL);

	while (1) {
		int dat = 1;
		gpio_get_value(0x26, &dat);
		if (dat != 0)
			goto next_run;

		uint16_t registers[6];
		if (modbus_read_registers(local_mb, 0, 6, registers) < 0)
			goto next_run;

		pthread_mutex_lock(&mutex);

		if (registers[5] == south || sleep_mode != 0)
			goto skip;

		time_t tmp = time(NULL);

		printf("%d %d %d %d %d\n", registers[5], last_last_point,
		       (unsigned)tmp, (unsigned)last_time, local_9);

		if ((registers[5] < south) && (local_9 == 0)) {
			local_9 = 1;
			last_last_point = south;
			last_last_time = action_time;
			action_time = tmp;
			printf("enter\n");
		} else if (registers[5] == last_last_point &&
			   (tmp - last_time) < 10 && local_9 == 1) {
			local_9 = 0;
			action_time = last_last_time;
			printf("exit\n");
		} else {
			action_time = tmp;
			printf("skip\n");
			if ((tmp - last_time) > 10 && local_9 == 1) {
				printf("leave\n");
				local_9 = 0;
			}
		}

		last_time = tmp;

skip:
		south = registers[5];
		pthread_mutex_unlock(&mutex);
		sync_flag = 1;

		switch (start_bit) {
		case 1:
			mb_mapping->tab_registers[3] = south;
			break;
		case 2:
			mb_mapping->tab_registers[1] = south;
			break;
		case 0:
			mb_mapping->tab_registers[2] = south;
		default:
			break;
		}

next_run:

		usleep(50000);
	}
}
