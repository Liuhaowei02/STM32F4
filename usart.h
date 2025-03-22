#ifndef __USART_H
#define __USART_H

#include "public.h"

void USART_Init_Config(void);
void Usart_SendByte(USART_TypeDef * pUSARTx , uint16_t ch);
void Usart_SendString(USART_TypeDef * pUSARTx , char* str);
void Serial_Printf(char *format, ...);
void Serial_SendString(char *String);
void Serial_SendByte(uint8_t Byte);
#endif
