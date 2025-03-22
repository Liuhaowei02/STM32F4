#ifndef __RS485_H
#define __RS485_H

#include "public.h"

void RS485_Init_Config(void);
void RS485_SendByte(uint8_t ch);
void RS485_SendString(char* str);


#endif


