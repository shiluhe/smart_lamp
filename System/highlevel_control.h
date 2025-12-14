#include "usart.h"
#include "string.h"
#include "pi_control.h"

#define RXBUFFERSIZE  256

void usart_send(char* string);
char esp8266_send_cmd(char *cmd,char *ack,uint16_t waittime);
void esp8266_test(void);
void esp8266_start_trans(void);
uint8_t esp8266_quit_trans(void);
uint8_t yaokong(void);
void mode_change(uint8_t my_mode);

