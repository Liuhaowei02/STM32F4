#ifndef __IIC_H
#define __IIC_H
#include "public.h"


//PE5:SCL   
#define RCC_IIC_SCL  RCC_AHB1Periph_GPIOE //端口时钟
#define IIC_SCL_PORT GPIOE  //端口号
#define IIC_SCL GPIO_Pin_5  //引脚

//PE6:SDA  
#define RCC_IIC_SDA RCC_AHB1Periph_GPIOE //端口时钟
#define IIC_SDA_PORT GPIOE  //端口号
#define IIC_SDA GPIO_Pin_6  //引脚


#define IIC_SDA_H GPIO_SetBits(IIC_SDA_PORT,IIC_SDA)    //SDA高电平
#define IIC_SDA_L GPIO_ResetBits(IIC_SDA_PORT,IIC_SDA)  //SDA低电平

#define IIC_SCL_H GPIO_SetBits(IIC_SCL_PORT,IIC_SCL)    //SCL高电平
#define IIC_SCL_L GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL)  //SCL低电平

#define READ_SDA GPIO_ReadInputDataBit(IIC_SDA_PORT,IIC_SDA)   //读取SDA高低电平

void IIC_GPIO_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_ACK(void);
void IIC_NACK(void);
void IIC_SendByte(uint8_t data);
uint8_t IIC_ReadByte(uint8_t ack);
uint8_t IIC_WaitACK(void);

#endif
