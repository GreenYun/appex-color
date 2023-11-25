#ifndef APP_COMM_H_
#define APP_COMM_H_

void *modbus_master_thread(void *_arg);
void *modbus_slave_thread(void *_arg);

#endif // APP_COMM_H_
