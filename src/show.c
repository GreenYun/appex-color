#include <stdio.h>

#include "show.h"

#include "lvgl/lvgl.h"

#include "global_var.h"

lv_obj_t *startup;
lv_obj_t *startup_label;
lv_obj_t *upgrade_all;
lv_obj_t *bg_all;
lv_obj_t *lcd_label;
lv_obj_t *num_bit1_all;
lv_obj_t *num_bit2_all;
lv_obj_t *num_bit3_all;
lv_obj_t *num_bit4_all;
lv_obj_t *num_bit5_all;
lv_obj_t *num_bit6_all;
lv_obj_t *num_bit1_south;
lv_obj_t *num_bit2_south;
lv_obj_t *num_bit3_south;
lv_obj_t *num_bit4_south;
lv_obj_t *num_bit5_south;
lv_obj_t *num_bit6_south;
lv_obj_t *num_bit7_south;
lv_obj_t *num_bit8_south;
lv_obj_t *num_bit9_south;
lv_obj_t *num_bit1_east;
lv_obj_t *num_bit2_east;
lv_obj_t *num_bit3_east;
lv_obj_t *num_bit4_east;
lv_obj_t *num_bit5_east;
lv_obj_t *num_bit6_east;
lv_obj_t *num_bit1_west;
lv_obj_t *num_bit2_west;
lv_obj_t *num_bit3_west;
lv_obj_t *num_bit4_west;
lv_obj_t *num_bit5_west;
lv_obj_t *num_bit6_west;
lv_obj_t *num_bit1_north;
lv_obj_t *num_bit2_north;
lv_obj_t *num_bit3_north;
lv_obj_t *num_bit4_north;
lv_obj_t *num_bit5_north;
lv_obj_t *num_bit6_north;
lv_obj_t *num_bit1_special;
lv_obj_t *num_bit2_special;
lv_obj_t *num_bit3_special;
lv_obj_t *num_bit4_special;
lv_obj_t *num_bit5_special;
lv_obj_t *num_bit6_special;
lv_obj_t *separate_south;
lv_obj_t *separate_east;
lv_obj_t *separate_west;
lv_obj_t *separate_north;
lv_obj_t *rank_south;
lv_obj_t *rank_east;
lv_obj_t *rank_west;
lv_obj_t *rank_north;
lv_obj_t *num_symbol_south;
lv_obj_t *num_symbol_east;
lv_obj_t *num_symbol_west;
lv_obj_t *num_symbol_north;
lv_obj_t *check_all;

int showpoint_create_obj(void)
{
	upgrade_all = lv_label_create(lv_scr_act(), NULL);
	lv_obj_set_hidden(upgrade_all, true);

	bg_all = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(bg_all, &showpoint_bg);

	// Used in show_lcd_time():
	lcd_label = lv_label_create(bg_all, NULL);
	lv_obj_set_pos(lcd_label, 0, 0);
	lv_obj_set_hidden(lcd_label, 1);

	// These fields seem to be unused:
	num_bit1_all = lv_img_create(bg_all, NULL);
	num_bit2_all = lv_img_create(bg_all, NULL);
	num_bit3_all = lv_img_create(bg_all, NULL);
	num_bit4_all = lv_img_create(bg_all, NULL);
	num_bit5_all = lv_img_create(bg_all, NULL);
	num_bit6_all = lv_img_create(bg_all, NULL);
	lv_obj_set_hidden(num_bit1_all, true);
	lv_obj_set_hidden(num_bit2_all, true);
	lv_obj_set_hidden(num_bit3_all, true);
	lv_obj_set_hidden(num_bit4_all, true);
	lv_obj_set_hidden(num_bit5_all, true);
	lv_obj_set_hidden(num_bit6_all, true);

	// Total points counter:
	num_bit1_special = lv_img_create(bg_all, NULL);
	num_bit2_special = lv_img_create(bg_all, NULL);
	num_bit3_special = lv_img_create(bg_all, NULL);
	num_bit4_special = lv_img_create(bg_all, NULL);
	num_bit5_special = lv_img_create(bg_all, NULL);
	num_bit6_special = lv_img_create(bg_all, NULL);
	lv_obj_set_hidden(num_bit1_special, true);
	lv_obj_set_hidden(num_bit2_special, true);
	lv_obj_set_hidden(num_bit3_special, true);
	lv_obj_set_hidden(num_bit4_special, true);
	lv_obj_set_hidden(num_bit5_special, true);
	lv_obj_set_hidden(num_bit6_special, true);
	lv_obj_set_pos(num_bit6_special, 17, 13);
	lv_obj_set_pos(num_bit5_special, 53, 13);
	lv_obj_set_pos(num_bit4_special, 89, 13);
	lv_obj_set_pos(num_bit3_special, 125, 13);
	lv_obj_set_pos(num_bit2_special, 161, 13);
	lv_obj_set_pos(num_bit1_special, 197, 13);

	check_all = lv_img_create(bg_all, NULL);
	lv_obj_set_hidden(check_all, 1);
	lv_obj_set_pos(check_all, 27, 26);

	// Players points counter:
	num_bit1_south = lv_img_create(bg_all, NULL);
	num_bit2_south = lv_img_create(bg_all, NULL);
	num_bit3_south = lv_img_create(bg_all, NULL);
	num_bit4_south = lv_img_create(bg_all, NULL);
	num_bit5_south = lv_img_create(bg_all, NULL);
	num_bit6_south = lv_img_create(bg_all, NULL);
	num_bit7_south = lv_img_create(bg_all, NULL);
	num_bit8_south = lv_img_create(bg_all, NULL);
	num_bit9_south = lv_img_create(bg_all, NULL);
	lv_obj_set_hidden(num_bit1_south, true);
	lv_obj_set_hidden(num_bit2_south, true);
	lv_obj_set_hidden(num_bit3_south, true);
	lv_obj_set_hidden(num_bit4_south, true);
	lv_obj_set_hidden(num_bit5_south, true);
	lv_obj_set_hidden(num_bit6_south, true);
	lv_obj_set_hidden(num_bit7_south, true);
	lv_obj_set_hidden(num_bit8_south, true);
	lv_obj_set_hidden(num_bit9_south, true);
	lv_obj_set_pos(num_bit6_south, 240, 366);
	lv_obj_set_pos(num_bit5_south, 310, 366);
	lv_obj_set_pos(num_bit4_south, 380, 366);
	lv_obj_set_pos(num_bit3_south, 450, 366);
	lv_obj_set_pos(num_bit2_south, 520, 391);
	lv_obj_set_pos(num_bit1_south, 573, 391);

	num_bit1_east = lv_img_create(bg_all, NULL);
	num_bit2_east = lv_img_create(bg_all, NULL);
	num_bit3_east = lv_img_create(bg_all, NULL);
	num_bit4_east = lv_img_create(bg_all, NULL);
	num_bit5_east = lv_img_create(bg_all, NULL);
	num_bit6_east = lv_img_create(bg_all, NULL);
	lv_obj_set_hidden(num_bit1_east, true);
	lv_obj_set_hidden(num_bit2_east, true);
	lv_obj_set_hidden(num_bit3_east, true);
	lv_obj_set_hidden(num_bit4_east, true);
	lv_obj_set_hidden(num_bit5_east, true);
	lv_obj_set_hidden(num_bit6_east, true);
	lv_obj_set_pos(num_bit6_east, 494, 212);
	lv_obj_set_pos(num_bit5_east, 552, 212);
	lv_obj_set_pos(num_bit4_east, 610, 212);
	lv_obj_set_pos(num_bit3_east, 668, 212);
	lv_obj_set_pos(num_bit2_east, 726, 234);
	lv_obj_set_pos(num_bit1_east, 771, 234);

	num_bit1_west = lv_img_create(bg_all, NULL);
	num_bit2_west = lv_img_create(bg_all, NULL);
	num_bit3_west = lv_img_create(bg_all, NULL);
	num_bit4_west = lv_img_create(bg_all, NULL);
	num_bit5_west = lv_img_create(bg_all, NULL);
	num_bit6_west = lv_img_create(bg_all, NULL);
	lv_obj_set_hidden(num_bit1_west, true);
	lv_obj_set_hidden(num_bit2_west, true);
	lv_obj_set_hidden(num_bit3_west, true);
	lv_obj_set_hidden(num_bit4_west, true);
	lv_obj_set_hidden(num_bit5_west, true);
	lv_obj_set_hidden(num_bit6_west, true);
	lv_obj_set_pos(num_bit6_west, 68, 212);
	lv_obj_set_pos(num_bit5_west, 126, 212);
	lv_obj_set_pos(num_bit4_west, 184, 212);
	lv_obj_set_pos(num_bit3_west, 242, 212);
	lv_obj_set_pos(num_bit2_west, 300, 234);
	lv_obj_set_pos(num_bit1_west, 345, 234);

	num_bit1_north = lv_img_create(bg_all, NULL);
	num_bit2_north = lv_img_create(bg_all, NULL);
	num_bit3_north = lv_img_create(bg_all, NULL);
	num_bit4_north = lv_img_create(bg_all, NULL);
	num_bit5_north = lv_img_create(bg_all, NULL);
	num_bit6_north = lv_img_create(bg_all, NULL);
	lv_obj_set_hidden(num_bit1_north, true);
	lv_obj_set_hidden(num_bit2_north, true);
	lv_obj_set_hidden(num_bit3_north, true);
	lv_obj_set_hidden(num_bit4_north, true);
	lv_obj_set_hidden(num_bit5_north, true);
	lv_obj_set_hidden(num_bit6_north, true);
	lv_obj_set_pos(num_bit6_north, 268, 57);
	lv_obj_set_pos(num_bit5_north, 326, 57);
	lv_obj_set_pos(num_bit4_north, 384, 57);
	lv_obj_set_pos(num_bit3_north, 442, 57);
	lv_obj_set_pos(num_bit2_north, 500, 79);
	lv_obj_set_pos(num_bit1_north, 545, 79);

	separate_south = lv_img_create(bg_all, NULL);
	separate_east = lv_img_create(bg_all, NULL);
	separate_west = lv_img_create(bg_all, NULL);
	separate_north = lv_img_create(bg_all, NULL);
	lv_img_set_src(separate_south, &myself_separate);
	lv_img_set_src(separate_east, &other_separate);
	lv_img_set_src(separate_west, &other_separate);
	lv_img_set_src(separate_north, &other_separate);
	lv_obj_set_pos(separate_south, 251, 336);
	lv_obj_set_pos(separate_west, 73, 185);
	lv_obj_set_pos(separate_east, 473, 185);
	lv_obj_set_pos(separate_north, 273, 30);

	rank_south = lv_img_create(bg_all, NULL);
	rank_east = lv_img_create(bg_all, NULL);
	rank_west = lv_img_create(bg_all, NULL);
	rank_north = lv_img_create(bg_all, NULL);
	lv_obj_set_hidden(rank_south, true);
	lv_obj_set_hidden(rank_east, true);
	lv_obj_set_hidden(rank_west, true);
	lv_obj_set_hidden(rank_north, true);
	lv_obj_set_pos(rank_south, 556, 315);
	lv_obj_set_pos(rank_west, 325, 158);
	lv_obj_set_pos(rank_east, 725, 158);
	lv_obj_set_pos(rank_north, 525, 3);

	num_symbol_south = lv_img_create(bg_all, NULL);
	num_symbol_east = lv_img_create(bg_all, NULL);
	num_symbol_west = lv_img_create(bg_all, NULL);
	num_symbol_north = lv_img_create(bg_all, NULL);
	lv_obj_set_hidden(num_symbol_south, true);
	lv_obj_set_hidden(num_symbol_east, true);
	lv_obj_set_hidden(num_symbol_west, true);
	lv_obj_set_hidden(num_symbol_north, true);

	printf("create obj succeed!\n");

	return 0;
}

unsigned int show_failed(unsigned int code)
{
	if ((code & 0xff) == 0)
		lv_label_set_text(startup_label, "wait for communication");
	else if ((code & 0xff) == 1)
		lv_label_set_text(startup_label, "device init failed");

	return 0;
}

void show_upgraded(int code)
{
	lv_obj_set_hidden(bg_all, true);
	lv_obj_set_hidden(upgrade_all, false);
	if (code == 0)
		lv_label_set_text(upgrade_all, "upgrade succeed!");
	else
		lv_label_set_text(upgrade_all, "upgrade failed!");
}

void show_upgrading(void)
{
	lv_obj_set_hidden(bg_all, true);
	lv_obj_set_hidden(upgrade_all, false);
	lv_label_set_text(upgrade_all, "upgrading!");
}

void hide_startup(void)
{
	lv_obj_set_hidden(startup, true);
}
