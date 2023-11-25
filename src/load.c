#include <stdio.h>
#include <stdlib.h>

#include "load.h"

#include "lvgl/lvgl.h"
#include "lv_lib_png/lodepng.h"

#include "global_var.h"

lv_img_dsc_t startup_img;
lv_img_dsc_t showpoint_bg;
lv_img_dsc_t myself_l_w_0;
lv_img_dsc_t myself_w_0;
lv_img_dsc_t myself_w_1;
lv_img_dsc_t myself_w_2;
lv_img_dsc_t myself_w_3;
lv_img_dsc_t myself_w_4;
lv_img_dsc_t myself_w_5;
lv_img_dsc_t myself_w_6;
lv_img_dsc_t myself_w_7;
lv_img_dsc_t myself_w_8;
lv_img_dsc_t myself_w_9;
lv_img_dsc_t other_l_w_0;
lv_img_dsc_t other_w_0;
lv_img_dsc_t other_w_1;
lv_img_dsc_t other_w_2;
lv_img_dsc_t other_w_3;
lv_img_dsc_t other_w_4;
lv_img_dsc_t other_w_5;
lv_img_dsc_t other_w_6;
lv_img_dsc_t other_w_7;
lv_img_dsc_t other_w_8;
lv_img_dsc_t other_w_9;
lv_img_dsc_t other_l_r_0;
lv_img_dsc_t other_r_0;
lv_img_dsc_t other_r_1;
lv_img_dsc_t other_r_2;
lv_img_dsc_t other_r_3;
lv_img_dsc_t other_r_4;
lv_img_dsc_t other_r_5;
lv_img_dsc_t other_r_6;
lv_img_dsc_t other_r_7;
lv_img_dsc_t other_r_8;
lv_img_dsc_t other_r_9;
lv_img_dsc_t other_l_b_0;
lv_img_dsc_t other_b_0;
lv_img_dsc_t other_b_1;
lv_img_dsc_t other_b_2;
lv_img_dsc_t other_b_3;
lv_img_dsc_t other_b_4;
lv_img_dsc_t other_b_5;
lv_img_dsc_t other_b_6;
lv_img_dsc_t other_b_7;
lv_img_dsc_t other_b_8;
lv_img_dsc_t other_b_9;
lv_img_dsc_t special_r_0;
lv_img_dsc_t special_r_1;
lv_img_dsc_t special_r_2;
lv_img_dsc_t special_r_3;
lv_img_dsc_t special_r_4;
lv_img_dsc_t special_r_5;
lv_img_dsc_t special_r_6;
lv_img_dsc_t special_r_7;
lv_img_dsc_t special_r_8;
lv_img_dsc_t special_r_9;
lv_img_dsc_t special_y_0;
lv_img_dsc_t special_y_1;
lv_img_dsc_t special_y_2;
lv_img_dsc_t special_y_3;
lv_img_dsc_t special_y_4;
lv_img_dsc_t special_y_5;
lv_img_dsc_t special_y_6;
lv_img_dsc_t special_y_7;
lv_img_dsc_t special_y_8;
lv_img_dsc_t special_y_9;
lv_img_dsc_t other_plus;
lv_img_dsc_t other_neg;
lv_img_dsc_t rank_1;
lv_img_dsc_t rank_2;
lv_img_dsc_t rank_3;
lv_img_dsc_t rank_4;
lv_img_dsc_t myself_separate;
lv_img_dsc_t other_separate;
lv_img_dsc_t check_all_yellow;
lv_img_dsc_t check_all_red;

#define LOAD_PNG_RESOURCE(name)                                               \
	do {                                                                  \
		ret = lodepng_load_file(&buf, &size,                          \
					"resources/" #name ".png");           \
		if (ret != 0) {                                               \
			printf("loadpng " #name " failed with: %d\n", ret);   \
			return ret;                                           \
		}                                                             \
		ret = lodepng_decode32(&decoded, &width, &height, buf, size); \
		if (ret != 0) {                                               \
			printf("decodepng " #name " failed with: %d\n", ret); \
			return ret;                                           \
		}                                                             \
		name.header.always_zero = 0;                                  \
		name.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;                  \
		name.header.w = width;                                        \
		name.header.h = height;                                       \
		name.data_size = width * height * 4;                          \
		name.data = decoded;                                          \
	} while (0)

int load_png_resource(void)
{
	unsigned ret, width, height;
	unsigned char *buf, *decoded;
	size_t size;

	LOAD_PNG_RESOURCE(showpoint_bg);
	LOAD_PNG_RESOURCE(myself_l_w_0);
	LOAD_PNG_RESOURCE(myself_w_0);
	LOAD_PNG_RESOURCE(myself_w_1);
	LOAD_PNG_RESOURCE(myself_w_2);
	LOAD_PNG_RESOURCE(myself_w_3);
	LOAD_PNG_RESOURCE(myself_w_4);
	LOAD_PNG_RESOURCE(myself_w_5);
	LOAD_PNG_RESOURCE(myself_w_6);
	LOAD_PNG_RESOURCE(myself_w_7);
	LOAD_PNG_RESOURCE(myself_w_8);
	LOAD_PNG_RESOURCE(myself_w_9);
	LOAD_PNG_RESOURCE(other_l_w_0);
	LOAD_PNG_RESOURCE(other_w_0);
	LOAD_PNG_RESOURCE(other_w_1);
	LOAD_PNG_RESOURCE(other_w_2);
	LOAD_PNG_RESOURCE(other_w_3);
	LOAD_PNG_RESOURCE(other_w_4);
	LOAD_PNG_RESOURCE(other_w_5);
	LOAD_PNG_RESOURCE(other_w_6);
	LOAD_PNG_RESOURCE(other_w_7);
	LOAD_PNG_RESOURCE(other_w_8);
	LOAD_PNG_RESOURCE(other_w_9);
	LOAD_PNG_RESOURCE(other_l_r_0);
	LOAD_PNG_RESOURCE(other_r_0);
	LOAD_PNG_RESOURCE(other_r_1);
	LOAD_PNG_RESOURCE(other_r_2);
	LOAD_PNG_RESOURCE(other_r_3);
	LOAD_PNG_RESOURCE(other_r_4);
	LOAD_PNG_RESOURCE(other_r_5);
	LOAD_PNG_RESOURCE(other_r_6);
	LOAD_PNG_RESOURCE(other_r_7);
	LOAD_PNG_RESOURCE(other_r_8);
	LOAD_PNG_RESOURCE(other_r_9);
	LOAD_PNG_RESOURCE(other_l_b_0);
	LOAD_PNG_RESOURCE(other_b_0);
	LOAD_PNG_RESOURCE(other_b_1);
	LOAD_PNG_RESOURCE(other_b_2);
	LOAD_PNG_RESOURCE(other_b_3);
	LOAD_PNG_RESOURCE(other_b_4);
	LOAD_PNG_RESOURCE(other_b_5);
	LOAD_PNG_RESOURCE(other_b_6);
	LOAD_PNG_RESOURCE(other_b_7);
	LOAD_PNG_RESOURCE(other_b_8);
	LOAD_PNG_RESOURCE(other_b_9);
	LOAD_PNG_RESOURCE(special_r_0);
	LOAD_PNG_RESOURCE(special_r_1);
	LOAD_PNG_RESOURCE(special_r_2);
	LOAD_PNG_RESOURCE(special_r_3);
	LOAD_PNG_RESOURCE(special_r_4);
	LOAD_PNG_RESOURCE(special_r_5);
	LOAD_PNG_RESOURCE(special_r_6);
	LOAD_PNG_RESOURCE(special_r_7);
	LOAD_PNG_RESOURCE(special_r_8);
	LOAD_PNG_RESOURCE(special_r_9);
	LOAD_PNG_RESOURCE(special_y_0);
	LOAD_PNG_RESOURCE(special_y_1);
	LOAD_PNG_RESOURCE(special_y_2);
	LOAD_PNG_RESOURCE(special_y_3);
	LOAD_PNG_RESOURCE(special_y_4);
	LOAD_PNG_RESOURCE(special_y_5);
	LOAD_PNG_RESOURCE(special_y_6);
	LOAD_PNG_RESOURCE(special_y_7);
	LOAD_PNG_RESOURCE(special_y_8);
	LOAD_PNG_RESOURCE(special_y_9);
	LOAD_PNG_RESOURCE(other_plus);
	LOAD_PNG_RESOURCE(other_neg);
	LOAD_PNG_RESOURCE(rank_1);
	LOAD_PNG_RESOURCE(rank_2);
	LOAD_PNG_RESOURCE(rank_3);
	LOAD_PNG_RESOURCE(rank_4);
	LOAD_PNG_RESOURCE(myself_separate);
	LOAD_PNG_RESOURCE(other_separate);
	LOAD_PNG_RESOURCE(check_all_yellow);
	LOAD_PNG_RESOURCE(check_all_red);

	printf("load png resources succeed!\n");

	return 0;
}

void load_startup(void)
{
	unsigned ret, width, height;
	unsigned char *buf, *decoded;
	size_t size;

	startup = lv_img_create(lv_scr_act(), NULL);

	ret = lodepng_load_file(&buf, &size, "resources/startup.png");
	if (ret != 0) {
		printf("loadpng startup failed with: %d\n", ret);
		return;
	}
	ret = lodepng_decode32(&decoded, &width, &height, buf, size);
	if (ret != 0) {
		printf("decodepng startup failed with: %d\n", ret);
		return;
	}
	startup_img.header.always_zero = 0;
	startup_img.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
	startup_img.header.w = width;
	startup_img.header.h = height;
	startup_img.data_size = width * height * 4;
	startup_img.data = decoded;

	lv_img_set_src(startup, &startup_img);
	lv_obj_set_hidden(startup, false);
	startup_label = lv_label_create(startup, NULL);
	lv_obj_set_pos(startup_label, 0, 0);
	lv_label_set_text(startup_label, "ver: Nov  8 2020");
}

#define LOAD_RANK(name)                                      \
	do {                                                 \
		for (i = 0; i < 4 && name != orders[i]; i++) \
			;                                    \
		switch (i) {                                 \
		case 0:                                      \
			lv_img_set_src(rank##name, &rank_1); \
			break;                               \
		case 1:                                      \
			lv_img_set_src(rank##name, &rank_2); \
			break;                               \
		case 2:                                      \
			lv_img_set_src(rank##name, &rank_3); \
			break;                               \
		case 3:                                      \
			lv_img_set_src(rank##name, &rank_4); \
		default:                                     \
			break;                               \
		}                                            \
		lv_obj_set_hidden(rank##name, false);        \
	} while (0)

#define LOAD_DIGIT(name, bit, style)                                   \
	do {                                                           \
		switch (digit) {                                       \
		case 0:                                                \
			lv_img_set_src(num_bit##bit##name, &style##0); \
			break;                                         \
		case 1:                                                \
			lv_img_set_src(num_bit##bit##name, &style##1); \
			break;                                         \
		case 2:                                                \
			lv_img_set_src(num_bit##bit##name, &style##2); \
			break;                                         \
		case 3:                                                \
			lv_img_set_src(num_bit##bit##name, &style##3); \
			break;                                         \
		case 4:                                                \
			lv_img_set_src(num_bit##bit##name, &style##4); \
			break;                                         \
		case 5:                                                \
			lv_img_set_src(num_bit##bit##name, &style##5); \
			break;                                         \
		case 6:                                                \
			lv_img_set_src(num_bit##bit##name, &style##6); \
			break;                                         \
		case 7:                                                \
			lv_img_set_src(num_bit##bit##name, &style##7); \
			break;                                         \
		case 8:                                                \
			lv_img_set_src(num_bit##bit##name, &style##8); \
			break;                                         \
		case 9:                                                \
			lv_img_set_src(num_bit##bit##name, &style##9); \
			break;                                         \
		default:                                               \
			lv_obj_set_hidden(num_bit##bit##name, true);   \
			continue;                                      \
			break;                                         \
		}                                                      \
		lv_obj_set_hidden(num_bit##bit##name, false);          \
	} while (0)

#define LOAD_NUM(name, pos, l, color)                              \
	do {                                                       \
		if (name < 1000)                                   \
			digit = 10;                                \
		else                                               \
			digit = (name) / 1000 % 10;                \
		LOAD_DIGIT(name, 6, pos##color);                   \
		if (name < 100)                                    \
			digit = 10;                                \
		else                                               \
			digit = (name) / 100 % 10;                 \
		LOAD_DIGIT(name, 5, pos##color);                   \
		if (name < 10)                                     \
			digit = 10;                                \
		else                                               \
			digit = (name) / 10 % 10;                  \
		LOAD_DIGIT(name, 4, pos##color);                   \
		digit = name % 10;                                 \
		LOAD_DIGIT(name, 3, pos##color);                   \
		lv_img_set_src(num_bit2##name, &pos##l##color##0); \
		lv_img_set_src(num_bit1##name, &pos##l##color##0); \
		lv_obj_set_hidden(num_bit2##name, false);          \
		lv_obj_set_hidden(num_bit1##name, false);          \
	} while (0)

#define LOAD_DIFF(name)                                                \
	do {                                                           \
		diff = (int)_south - (int)name;                        \
		if (diff == 0) {                                       \
			lv_obj_set_hidden(num_symbol##name, true);     \
			lv_obj_set_hidden(num_bit6##name, true);       \
			lv_obj_set_hidden(num_bit5##name, true);       \
			lv_obj_set_hidden(num_bit4##name, true);       \
			lv_img_set_src(num_bit3##name, &other_w_0);    \
			lv_img_set_src(num_bit2##name, &other_l_w_0);  \
			lv_img_set_src(num_bit1##name, &other_l_w_0);  \
			lv_obj_set_hidden(num_bit3##name, false);      \
			lv_obj_set_hidden(num_bit2##name, false);      \
			lv_obj_set_hidden(num_bit1##name, false);      \
		} else if (diff < 0) {                                 \
			unsigned short tmp = name;                     \
			name = (unsigned short)(-diff);                \
			LOAD_NUM(name, other_, l_, r_);                \
			lv_img_set_src(num_symbol##name, &other_neg);  \
			name = tmp;                                    \
		} else {                                               \
			unsigned short tmp = name;                     \
			name = diff;                                   \
			LOAD_NUM(name, other_, l_, b_);                \
			lv_img_set_src(num_symbol##name, &other_plus); \
			name = tmp;                                    \
		}                                                      \
	} while (0)

// Due to compilation optimization, the logic below has been rewritten instead
// of copying from the reversed.
int load_num(int _east, int _west, int _south, int _north,
	     unsigned char _check_flag, __attribute__((unused)) int _some_flag,
	     unsigned char _show_state)
{
	unsigned short total = _east + _west + _south + _north;
	unsigned short orders[5] = { _east, _west, _south, _north };

	int i;
	unsigned char digit;

	for (i = 0; i < 3; i++) {
		for (int j = 0; j < 3 - i; j++) {
			if (orders[j] < orders[j + 1]) {
				unsigned short tmp = orders[j];
				orders[j] = orders[j + 1];
				orders[j + 1] = tmp;
			}
		}
	}

	LOAD_RANK(_east);
	LOAD_RANK(_west);
	LOAD_RANK(_north);
	LOAD_RANK(_south);

	// Show points
	if (_show_state == 0) {
		lv_obj_set_hidden(num_symbol_east, true);
		lv_obj_set_hidden(num_symbol_west, true);
		lv_obj_set_hidden(num_symbol_north, true);

		LOAD_NUM(_west, other_, l_, w_);
		LOAD_NUM(_north, other_, l_, w_);
		LOAD_NUM(_east, other_, l_, w_);
	}
	// Show differences
	else {
		lv_obj_set_hidden(num_symbol_east, false);
		lv_obj_set_hidden(num_symbol_west, false);
		lv_obj_set_hidden(num_symbol_north, false);

		int diff;
		LOAD_DIFF(_west);
		if (diff < 1000)
			lv_obj_set_pos(num_symbol_west, 77, 233);
		else
			lv_obj_set_pos(num_symbol_west, 17, 233);

		LOAD_DIFF(_north);
		if (diff < 1000)
			lv_obj_set_pos(num_symbol_north, 277, 78);
		else
			lv_obj_set_pos(num_symbol_north, 217, 78);

		LOAD_DIFF(_east);
		if (diff < 1000)
			lv_obj_set_pos(num_symbol_east, 477, 233);
		else
			lv_obj_set_pos(num_symbol_east, 443, 233);
	}

	// Self points

	// Digit 7~9 to show LCD uptime.
	if (_south >= 1000000)
		digit = _south / 1000000 % 10;
	else
		digit = 10;
	LOAD_DIGIT(_south, 9, myself_w_);
	if (_south >= 100000)
		digit = _south / 100000 % 10;
	else
		digit = 10;
	LOAD_DIGIT(_south, 8, myself_w_);
	if (_south >= 10000)
		digit = _south / 10000 % 10;
	else
		digit = 10;
	LOAD_DIGIT(_south, 7, myself_w_);

	LOAD_NUM(_south, myself_, l_, w_);

	// Below duplicates showpoint_create_obj() with minor changes.
	if (_north >= 1000)
		lv_obj_set_pos(num_bit6_north, 268, 57);
	lv_obj_set_pos(num_bit5_north, 326, 57);
	lv_obj_set_pos(num_bit4_north, 384, 57);
	lv_obj_set_pos(num_bit3_north, 442, 57);
	lv_obj_set_pos(num_bit2_north, 500, 79);
	lv_obj_set_pos(num_bit1_north, 545, 79);
	lv_obj_set_pos(separate_north, 273, 30);
	lv_obj_set_pos(rank_north, 525, 3);

	if (_south >= 1000) {
		lv_obj_set_pos(num_bit9_south, 30, 366);
		lv_obj_set_pos(num_bit8_south, 100, 366);
		lv_obj_set_pos(num_bit7_south, 170, 366);
		lv_obj_set_pos(num_bit6_south, 240, 366);
	}
	lv_obj_set_pos(num_bit5_south, 310, 366);
	lv_obj_set_pos(num_bit4_south, 380, 366);
	lv_obj_set_pos(num_bit3_south, 450, 366);
	lv_obj_set_pos(num_bit2_south, 520, 391);
	lv_obj_set_pos(num_bit1_south, 573, 391);
	lv_obj_set_pos(separate_south, 251, 336);
	lv_obj_set_pos(rank_south, 556, 315);

	if (_east >= 1000)
		lv_obj_set_pos(num_bit6_east, 494, 212);
	lv_obj_set_pos(num_bit5_east, 552, 212);
	lv_obj_set_pos(num_bit4_east, 610, 212);
	lv_obj_set_pos(num_bit3_east, 668, 212);
	lv_obj_set_pos(num_bit2_east, 726, 234);
	lv_obj_set_pos(num_bit1_east, 771, 234);
	lv_obj_set_pos(separate_east, 499, 185); // change
	lv_obj_set_pos(rank_east, 751, 158); // change

	if (_west >= 1000)
		lv_obj_set_pos(num_bit6_west, 68, 212);
	lv_obj_set_pos(num_bit5_west, 126, 212);
	lv_obj_set_pos(num_bit4_west, 184, 212);
	lv_obj_set_pos(num_bit3_west, 242, 212);
	lv_obj_set_pos(num_bit2_west, 300, 234);
	lv_obj_set_pos(num_bit1_west, 345, 234);
	lv_obj_set_pos(separate_west, 73, 185);
	lv_obj_set_pos(rank_west, 325, 158);

	// Check all
	if (_check_flag < 2 && _show_state == 0) {
		lv_obj_set_hidden(num_bit1_special, true);
		lv_obj_set_hidden(num_bit2_special, true);
		lv_obj_set_hidden(num_bit3_special, true);
		lv_obj_set_hidden(num_bit4_special, true);
		lv_obj_set_hidden(num_bit5_special, true);
		lv_obj_set_hidden(num_bit6_special, true);
		lv_obj_set_hidden(check_all, false);
		if (_check_flag == 1)
			lv_img_set_src(check_all, &check_all_yellow);
		else
			lv_img_set_src(check_all, &check_all_red);

	} else {
		lv_obj_set_hidden(check_all, true);
		unsigned short _special = total;
		if ((_check_flag & 0x01) == 0)
			LOAD_NUM(_special, special_, , r_);
		else
			LOAD_NUM(_special, special_, , y_);
	}

	if (default_east == 0) {
		lv_obj_set_hidden(num_bit6_east, true);
		lv_obj_set_hidden(num_bit5_east, true);
		lv_obj_set_hidden(num_bit4_east, true);
		lv_obj_set_hidden(num_bit3_east, true);
		lv_obj_set_hidden(num_bit2_east, true);
		lv_obj_set_hidden(num_bit1_east, true);
		lv_obj_set_hidden(rank_east, true);
		lv_obj_set_hidden(num_symbol_east, true);
		lv_obj_set_pos(separate_east, 499, 185);
	}
	if (default_west == 0) {
		lv_obj_set_hidden(num_bit6_west, true);
		lv_obj_set_hidden(num_bit5_west, true);
		lv_obj_set_hidden(num_bit4_west, true);
		lv_obj_set_hidden(num_bit3_west, true);
		lv_obj_set_hidden(num_bit2_west, true);
		lv_obj_set_hidden(num_bit1_west, true);
		lv_obj_set_hidden(rank_west, true);
		lv_obj_set_hidden(num_symbol_west, true);
		lv_obj_set_pos(separate_west, 73, 185);
	}
	if (default_north == 0) {
		lv_obj_set_hidden(num_bit6_north, true);
		lv_obj_set_hidden(num_bit5_north, true);
		lv_obj_set_hidden(num_bit4_north, true);
		lv_obj_set_hidden(num_bit3_north, true);
		lv_obj_set_hidden(num_bit2_north, true);
		lv_obj_set_hidden(num_bit1_north, true);
		lv_obj_set_hidden(rank_north, true);
		lv_obj_set_hidden(num_symbol_north, true);
		lv_obj_set_pos(separate_north, 273, 30);
	}

	return 0;
}
