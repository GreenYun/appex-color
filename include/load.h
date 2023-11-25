#ifndef APP_LOAD_H
#define APP_LOAD_H

int load_png_resource(void);
void load_startup(void);

int load_num(int _east, int _west, int _south, int _north,
	     unsigned char _check_flag, int _some_flag,
	     unsigned char _show_state);

#endif // APP_LOAD_H
