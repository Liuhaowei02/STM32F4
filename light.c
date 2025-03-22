#include "public.h"

uint16_t BH1750_ReadData(void)
{
	uint16_t H_Data;  //高八位
	uint8_t L_Data;  //低八位	
	uint16_t  R_Data;  //完整16位数据
	
	IIC_Start();           //发送开始信号
	
	IIC_SendByte(BH1750_ADDRESS<<1 | 0);   //发送BH1750七位地址加读写位（写操作
	
	if(IIC_WaitACK()==0)     //返回0应答成功
	{
		IIC_SendByte(BH1750_CMD_RESOLUTION);  //发送分辨率设置指令：分辨率位1
		if(IIC_WaitACK()==0)   //应答成功
		{
			IIC_Stop();
		}
	}
	Delay_ms(BH1750_LIGHT_COLLECTION_TIME);  //等待光照传感器采集光照强度
	
	IIC_Start();   //重新发送开始信号
	IIC_SendByte(BH1750_ADDRESS<<1 | 1);   //发送BH1750七位地址加读写位（读操作
	
	if(IIC_WaitACK()==0)
	{
		H_Data = IIC_ReadByte(0);  //读取高八位并应答 0表示还要接着读取
		L_Data = IIC_ReadByte(1);  //读取低八位并应答
	
		IIC_Stop();                //读取完毕，释放总线
	}
	
	R_Data = H_Data<<8 | L_Data; //完整十六位数据
	
	return R_Data;
}
