 #ifndef __DHT11_H
 #define __DHT11_H
 #include "stm32f4xx.h"

 #define dht11_high GPIO_SetBits(GPIOD, GPIO_Pin_0)
 #define dht11_low GPIO_ResetBits(GPIOD, GPIO_Pin_0)
 #define Read_Data GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0)

 void DHT11_GPIO_Init_OUT(void);
 void DHT11_GPIO_Init_IN(void);
 void DHT11_Start(void);
 char DHT11_Rec_Byte(void);
void DHT11_REC_Data(void);




 #endif

