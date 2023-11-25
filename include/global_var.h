#ifndef APP_GLOBAL_VAR_H_
#define APP_GLOBAL_VAR_H_

#include <pthread.h>
#include <stdint.h>
#include <time.h>

#include <modbus/modbus.h>

#include "lvgl/lvgl.h"

extern unsigned short south;
extern unsigned short east;
extern unsigned short west;
extern unsigned short north;

extern unsigned short dummy;

extern unsigned short default_south;
extern unsigned short default_east;
extern unsigned short default_west;
extern unsigned short default_north;

extern int lcd_uptime;
extern int lcd_status;
extern int header_length;

extern short action_flag;
extern unsigned char show_state;
extern unsigned char start_flag;
extern unsigned char sync_flag;
extern unsigned char check_flag;
extern unsigned char sleep_mode;
extern unsigned char reading;
extern unsigned char special_start;
extern unsigned char start_bit;

extern pthread_mutex_t mutex;

extern time_t action_time;
extern time_t now_time;

extern modbus_t *local_mb;
extern modbus_t *remote_mb;

extern modbus_mapping_t *mb_mapping;

// time in telecomm.c
extern unsigned long remote_action_time;
extern unsigned long remote_now_time;

// `last`s in read.c
extern time_t last_time;
extern time_t last_last_time;
extern unsigned short last_point;
extern unsigned short last_last_point;

// Image data variable in load.c
extern lv_img_dsc_t startup_img;
extern lv_img_dsc_t showpoint_bg;
extern lv_img_dsc_t myself_l_w_0;
extern lv_img_dsc_t myself_w_0;
extern lv_img_dsc_t myself_w_1;
extern lv_img_dsc_t myself_w_2;
extern lv_img_dsc_t myself_w_3;
extern lv_img_dsc_t myself_w_4;
extern lv_img_dsc_t myself_w_5;
extern lv_img_dsc_t myself_w_6;
extern lv_img_dsc_t myself_w_7;
extern lv_img_dsc_t myself_w_8;
extern lv_img_dsc_t myself_w_9;
extern lv_img_dsc_t other_l_w_0;
extern lv_img_dsc_t other_w_0;
extern lv_img_dsc_t other_w_1;
extern lv_img_dsc_t other_w_2;
extern lv_img_dsc_t other_w_3;
extern lv_img_dsc_t other_w_4;
extern lv_img_dsc_t other_w_5;
extern lv_img_dsc_t other_w_6;
extern lv_img_dsc_t other_w_7;
extern lv_img_dsc_t other_w_8;
extern lv_img_dsc_t other_w_9;
extern lv_img_dsc_t other_l_r_0;
extern lv_img_dsc_t other_r_0;
extern lv_img_dsc_t other_r_1;
extern lv_img_dsc_t other_r_2;
extern lv_img_dsc_t other_r_3;
extern lv_img_dsc_t other_r_4;
extern lv_img_dsc_t other_r_5;
extern lv_img_dsc_t other_r_6;
extern lv_img_dsc_t other_r_7;
extern lv_img_dsc_t other_r_8;
extern lv_img_dsc_t other_r_9;
extern lv_img_dsc_t other_l_b_0;
extern lv_img_dsc_t other_b_0;
extern lv_img_dsc_t other_b_1;
extern lv_img_dsc_t other_b_2;
extern lv_img_dsc_t other_b_3;
extern lv_img_dsc_t other_b_4;
extern lv_img_dsc_t other_b_5;
extern lv_img_dsc_t other_b_6;
extern lv_img_dsc_t other_b_7;
extern lv_img_dsc_t other_b_8;
extern lv_img_dsc_t other_b_9;
extern lv_img_dsc_t special_r_0;
extern lv_img_dsc_t special_r_1;
extern lv_img_dsc_t special_r_2;
extern lv_img_dsc_t special_r_3;
extern lv_img_dsc_t special_r_4;
extern lv_img_dsc_t special_r_5;
extern lv_img_dsc_t special_r_6;
extern lv_img_dsc_t special_r_7;
extern lv_img_dsc_t special_r_8;
extern lv_img_dsc_t special_r_9;
extern lv_img_dsc_t special_y_0;
extern lv_img_dsc_t special_y_1;
extern lv_img_dsc_t special_y_2;
extern lv_img_dsc_t special_y_3;
extern lv_img_dsc_t special_y_4;
extern lv_img_dsc_t special_y_5;
extern lv_img_dsc_t special_y_6;
extern lv_img_dsc_t special_y_7;
extern lv_img_dsc_t special_y_8;
extern lv_img_dsc_t special_y_9;
extern lv_img_dsc_t other_plus;
extern lv_img_dsc_t other_neg;
extern lv_img_dsc_t rank_1;
extern lv_img_dsc_t rank_2;
extern lv_img_dsc_t rank_3;
extern lv_img_dsc_t rank_4;
extern lv_img_dsc_t myself_separate;
extern lv_img_dsc_t other_separate;
extern lv_img_dsc_t check_all_yellow;
extern lv_img_dsc_t check_all_red;

// Objects in show.c
extern lv_obj_t *startup;
extern lv_obj_t *startup_label;
extern lv_obj_t *upgrade_all;
extern lv_obj_t *bg_all;
extern lv_obj_t *lcd_label;
extern lv_obj_t *num_bit1_all;
extern lv_obj_t *num_bit2_all;
extern lv_obj_t *num_bit3_all;
extern lv_obj_t *num_bit4_all;
extern lv_obj_t *num_bit5_all;
extern lv_obj_t *num_bit6_all;
extern lv_obj_t *num_bit1_south;
extern lv_obj_t *num_bit2_south;
extern lv_obj_t *num_bit3_south;
extern lv_obj_t *num_bit4_south;
extern lv_obj_t *num_bit5_south;
extern lv_obj_t *num_bit6_south;
extern lv_obj_t *num_bit7_south;
extern lv_obj_t *num_bit8_south;
extern lv_obj_t *num_bit9_south;
extern lv_obj_t *num_bit1_east;
extern lv_obj_t *num_bit2_east;
extern lv_obj_t *num_bit3_east;
extern lv_obj_t *num_bit4_east;
extern lv_obj_t *num_bit5_east;
extern lv_obj_t *num_bit6_east;
extern lv_obj_t *num_bit1_west;
extern lv_obj_t *num_bit2_west;
extern lv_obj_t *num_bit3_west;
extern lv_obj_t *num_bit4_west;
extern lv_obj_t *num_bit5_west;
extern lv_obj_t *num_bit6_west;
extern lv_obj_t *num_bit1_north;
extern lv_obj_t *num_bit2_north;
extern lv_obj_t *num_bit3_north;
extern lv_obj_t *num_bit4_north;
extern lv_obj_t *num_bit5_north;
extern lv_obj_t *num_bit6_north;
extern lv_obj_t *num_bit1_special;
extern lv_obj_t *num_bit2_special;
extern lv_obj_t *num_bit3_special;
extern lv_obj_t *num_bit4_special;
extern lv_obj_t *num_bit5_special;
extern lv_obj_t *num_bit6_special;
extern lv_obj_t *separate_south;
extern lv_obj_t *separate_east;
extern lv_obj_t *separate_west;
extern lv_obj_t *separate_north;
extern lv_obj_t *rank_south;
extern lv_obj_t *rank_east;
extern lv_obj_t *rank_west;
extern lv_obj_t *rank_north;
extern lv_obj_t *num_symbol_south;
extern lv_obj_t *num_symbol_east;
extern lv_obj_t *num_symbol_west;
extern lv_obj_t *num_symbol_north;
extern lv_obj_t *check_all;

#endif // APP_GLOBAL_VAR_H_
