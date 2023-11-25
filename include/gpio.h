#ifndef APP_GPIO_H
#define APP_GPIO_H

#define GPIO_DIR_IN 0
#define GPIO_DIR_OUT 1

int gpio_export(unsigned int gpio);
int gpio_set_dir(unsigned int gpio, int flag);

int gpio_get_value(unsigned int gpio, int *val);
int gpio_set_value(unsigned int gpio, int val);

#endif // APP_GPIO_H
