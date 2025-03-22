#ifndef __KEY_H
#define __KEY_H
#include <stdbool.h>
#include "stm32f4xx.h"
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KEY2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)


void KEY_Init(void);
bool GetStaKey(void);
uint8_t KEY_Scan(void);



#endif

