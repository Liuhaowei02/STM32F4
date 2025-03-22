#include "public.h"
//发送数据
void BH1750_Write(void)
{ 
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));        //等待IIC总线空闲
	I2C_GenerateSTART(I2C1,ENABLE);											 //发送开始信号
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));   //等待开始信号发送完成
	I2C_Send7bitAddress(I2C1,BH1750_ADDRESS<<1,I2C_Direction_Transmitter);  //发送从设备地址+写操作
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); //等待地址发送完成
	
	I2C_SendData(I2C1,BH1750_CMD_RESOLUTION);                                //发送命令
	
	
	
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //等待命令发送完成
	
	
	I2C_GenerateSTOP(I2C1,ENABLE);                                   //发送停止信号
}

//从BH1750接收数据
uint16_t BH1750_READ(void)
{
	uint8_t data[2];
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));                   //等待IIC总线空闲
	I2C_GenerateSTART(I2C1,ENABLE);											 //发送开始信号

	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));   //等待开始信号发送完成
	I2C_Send7bitAddress(I2C1,BH1750_ADDRESS<<1,I2C_Direction_Receiver);  //发送从设备地址+读操作
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); //等待地址发送完成	
	
	//接收第一个字符
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE)==RESET);                  //判定SDA字符有效 判定数据寄存器非空
	data[0]=I2C_ReceiveData(I2C1);
	
	//接收第二个字符
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE)==RESET);                  //判定SDA字符有效 判定数据寄存器非空
	data[1]=I2C_ReceiveData(I2C1);
	
	//发送停止信号
	I2C_GenerateSTOP(I2C1,ENABLE); 
	
	//合并高八位和低八位数据，并返回
	return data[0]<<8 | data[1];

}
