#include "highlevel_control.h"

char espBuffer[RXBUFFERSIZE];  

char RxBuffer[RXBUFFERSIZE];  
uint8_t aRxBuffer;		
uint8_t Uart3_Rx_Cnt = 0;	
uint8_t my_mode = 0 ;

char my_order[10]={0};
char receive_flag=0;

volatile uint8_t voice_cmd_ready = 0; 
uint8_t voice_rx_buffer[5] = {0};

float light_value = 0.0f;

void usart_send(char* string){
    uint8_t num=0;
    char my_ch[50]={0};
    while(*string!=0)
        my_ch[num++]=*string++;

    my_ch[num++]='\r';
    my_ch[num++]='\n';

    HAL_UART_Transmit(&huart3, (uint8_t *)my_ch,num, 0xffff);
    while(HAL_UART_GetState(&huart3) == HAL_UART_STATE_BUSY_TX);
}

char esp8266_send_cmd(char *cmd,char *ack,uint16_t waittime){

    usart_send(cmd);

    if(ack&&waittime){
        while(--waittime){
            HAL_Delay(10);
            if(strstr((const char*)espBuffer,(const char*)ack)){
                memset(espBuffer,0x00,sizeof(espBuffer));
                return 1;
            }
        }
    }
    return 0;
}

void esp8266_test(){
    esp8266_send_cmd("AT","OK",50);
}

void esp8266_start_trans(){

    esp8266_send_cmd("AT+CWMODE=2","OK",50);
    esp8266_send_cmd("AT+RST","ready",20);

    HAL_Delay(3000);

    esp8266_send_cmd("AT+CWSAP=\"esp8266\",\"123456789\",1,4","OK",200);
    esp8266_send_cmd("AT+CIPMUX=1","OK",20);
    esp8266_send_cmd("AT+CIPSERVER=1,8080","OK",200);
    //esp8266_send_cmd("AT+CIPSEND","OK",50);
}

uint8_t esp8266_quit_trans(){
    uint8_t result=1;
    usart_send("+++");
    HAL_Delay(1000);
    result=esp8266_send_cmd("AT","OK",20);
    return result;
}
//////////////////////////////////////////////////////////////////////////////
uint8_t yaokong(){
		if(receive_flag){
			receive_flag=0;
			if(strstr((const char*)my_order,(const char*)"1")){
					Uart3_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer));
					memset(my_order,0x00,sizeof(my_order));
					my_mode = 1 ;
			}
			else if(strstr((const char*)my_order,(const char*)"2")){
					Uart3_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer));
					memset(my_order,0x00,sizeof(my_order));
					my_mode = 2 ;
			}
			else if(strstr((const char*)my_order,(const char*)"3")){
					Uart3_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); 
					memset(my_order,0x00,sizeof(my_order));
					my_mode = 3 ;
			}
			else if(strstr((const char*)my_order,(const char*)"4")){
					Uart3_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); 
					memset(my_order,0x00,sizeof(my_order));
					my_mode = 4 ;
		}
			else if(strstr((const char*)my_order,(const char*)"5")){
					Uart3_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); 
					memset(my_order,0x00,sizeof(my_order));
					my_mode = 5 ;
		}
			else if(strstr((const char*)my_order,(const char*)"6")){
					Uart3_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); 
					memset(my_order,0x00,sizeof(my_order));
					my_mode = 6 ;
		}
	}
			return my_mode;
}

void mode_change(uint8_t my_mode){
	switch(my_mode){
		case 1:
				huxi();
			break;
		case 2:
				off_lamp();
			break;
		case 3:
				on_lamp();
			break;
		case 4:
			  baoshan();
		case 5:
				DO_lamp();
		case 6:
				AO_lamp(light_value);
		default:
			break;
	}	
}
///////////////////////////////////////////////////////////////////
void Voice_I2C_Read_Start(){
    if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY){
        HAL_I2C_Mem_Read_IT(&hi2c1, VOICE_SLAVE_ADDR << 1, VOICE_REG_ADDR,  I2C_MEMADD_SIZE_8BIT, voice_rx_buffer,1);  			
    }
}
uint8_t yuyin(){
			static uint32_t last_read_time = 0;
    if (HAL_GetTick() - last_read_time >= 50){
        last_read_time = HAL_GetTick();
        Voice_I2C_Read_Start();
    }
    if (voice_cmd_ready){
			voice_cmd_ready = 0;
			uint8_t cmd_id = voice_rx_buffer[0]; 
			//char buf[100];
			//sprintf(buf, "Voice CMD ID: %02X\r\n", cmd_id); 
			//HAL_UART_Transmit(&huart3, (uint8_t*)buf, strlen(buf), 100);
			if (cmd_id != 0x00){
					if (cmd_id == 0x12){
						my_mode = 3;
					}
					else if (cmd_id == 0x13){
						my_mode = 2;
					}
					else if (cmd_id == 0x7C){
						my_mode = 1;
					}
					else if (cmd_id == 0x7D){
						my_mode = 4;
					}
			 }	
    }
		return my_mode;
}




