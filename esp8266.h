#ifndef __ESP8266_H
#define __ESP8266_H

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志
#include <stm32f4xx.h>

void ESP8266_Init(void);





void USART3WIFI_Init_Config(void);


#endif
