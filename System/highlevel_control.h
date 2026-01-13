#include "string.h"
#include "pi_control.h"
#include "usart.h"
#include "i2c.h"

#define RXBUFFERSIZE  256

#define VOICE_SLAVE_ADDR    0x34  
#define VOICE_REG_ADDR      0x64 

void usart_send(char* string);
char esp8266_send_cmd(char *cmd,char *ack,uint16_t waittime);
void esp8266_test(void);
void esp8266_start_trans(void);
uint8_t esp8266_quit_trans(void);
uint8_t yaokong(void);

void Voice_I2C_Read_Start(void);
uint8_t yuyin(void);

void mode_change(uint8_t my_mode);
void mode_change_motor(uint8_t my_mode_motor);
