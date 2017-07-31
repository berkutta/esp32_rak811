#ifndef __RAK811_H__
#define __RAK811_H__

#include "driver/uart.h"

#define uart_interface UART_NUM_1
#define uart_buf_size 1024

void rak811_init(uint8_t tx, uint8_t rx);
void rak811_send_cmd_response(char *response, char *cmd);
int rak811_send_cmd_cmp(char *cmp, char *cmd);
void rak811_send_cmd(char *cmd);
void rak811_version(void);
void rak811_sleep(void);
void rak811_wakeup(void);
void rak811_mode(uint8_t mode);
void rak811_set_app_eui(char *app_eui);
void rak811_set_app_key(char *app_key);

#endif