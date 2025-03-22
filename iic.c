#include "public.h"
//光敏模块IIC配置
//软件模拟IIC  PE5->SCL, PE6->SDA
void IIC_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_IIC_SCL,ENABLE);   //使能SCL引脚时钟
	RCC_AHB1PeriphClockCmd(RCC_IIC_SDA,ENABLE);   //使能SDA引脚时钟
	
	//配置SCL引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;      //输出模式 
	GPIO_InitStructure.GPIO_Pin=IIC_SCL;					   //PE5->SCL
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			 //上拉模式	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;     //开漏输出
	
	GPIO_Init(IIC_SCL_PORT,&GPIO_InitStructure);     //初始化SCL引脚
	
	//配置SDA引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;      //输出模式 
	GPIO_InitStructure.GPIO_Pin= IIC_SDA;   				 //PE6->SDA
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			 //上拉模式	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;     //开漏输出
	
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);     //初始化SDA引脚
	
	//默认SCL、SDA高电平
	IIC_SDA_H;
	IIC_SCL_H;
	
}


static void IIC_SDA_OUT(void) 														//配置SDA为输出模式：发送数据（写）
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	//配置SDA引脚为输出模式
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;      //输出模式 
	GPIO_InitStructure.GPIO_Pin= IIC_SDA;   				 //PE6->SDA
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			 //上拉模式	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;     //开漏输出
	
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);     //初始化SDA引脚	
	
}


static void IIC_SDA_IN(void)															//配置SDA为输入模式：接收数据（读）
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	//配置SDA引脚为输出模式
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;      //输入模式 
	GPIO_InitStructure.GPIO_Pin= IIC_SDA;   				 //PE6->SDA
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			 //上拉模式	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);     //初始化SDA引脚	
	
}


void IIC_Start(void)           //IIC开始信号
{
	IIC_SDA_OUT();

	IIC_SCL_H;                   //拉高SCL线
	IIC_SDA_H;                   //拉高SDA线
	Delay_us(4);                 //让高电平保持平稳
	
	IIC_SDA_L;                   //保持SCL高电平状态，拉低SDA,产生下降沿：开始信号
	
	Delay_us(4);
	
	IIC_SCL_L;									 //准备发送数据
}


void IIC_Stop(void)						 //IIC停止信号
{
	IIC_SDA_OUT();
	

	IIC_SCL_H;                   //拉高SCL线
	IIC_SDA_L;                   //拉低SDA线	
	Delay_us(4);
	
	IIC_SDA_H;									 //拉高SDA，产生上升沿：结束信号
	
}


void IIC_ACK(void)             //IIC应答信号
{
	IIC_SDA_OUT();
	IIC_SCL_L;									 //SCL为低电平状态下，SDA开始准备数据
	IIC_SDA_L;                   //拉低SDA线，从机准备应答位：0，表示接收成功
	
	Delay_us(1);
	
	IIC_SCL_H;                   //SCL高电平,应答位有效,如果是0表示接受成功
	Delay_us(1);
	
	IIC_SCL_L;                   //一个周期结束，等待下个周期数据准备
	
}

void IIC_NACK(void)
{
	IIC_SDA_OUT();
	IIC_SCL_L;									 //SCL为低电平状态下，SDA开始准备数据
	IIC_SDA_H;                   //拉低SDA线，从机准备应答位：1，表示接收失败
	
	Delay_us(1);
	
	IIC_SCL_H;                   //SCL高电平,应答位有效,如果是1表示接受失败
	Delay_us(1);	
	
	IIC_SCL_L;                   //一个周期结束，等待下个周期数据准备
}


void IIC_SendByte(uint8_t data)        //发送一个字节数据
{
	IIC_SDA_OUT();
	IIC_SCL_L;                           //在SCL为低电平下，SDA准备数据 
	
	uint8_t i=0;
	for(i=0;i<8;i++)
	{
		if(data &0x80)                    //判断第一位是不是1
		{
			IIC_SDA_H;	                    //拉高SDA
		}
		else
		{
			IIC_SDA_L;                      //SDA低电平
		}
		IIC_SCL_H;                        //拉高SCL,数据有效
		Delay_us(1);
		
		IIC_SCL_L;                        //拉低SCL,等待SDA准备下一位数据
		Delay_us(1);
		
		data<<=1;                     //移位
	}
}


uint8_t IIC_ReadByte(uint8_t ack)       //接收一个字节
{
	IIC_SDA_IN();
	
	uint8_t i=0;
	uint8_t receive=0;
	for(i=0;i<8;i++)
	{	
		IIC_SCL_L;											//SCL拉低，等待从机准备数据
		Delay_us(1);
		IIC_SCL_H;                      //拉高SCL，数据有效
		receive<<=1;
		if(READ_SDA)
		{
			receive++;
		}
		Delay_us(1);
	}
		if(ack)
		{
			IIC_NACK();
		}
		else
		{
			IIC_ACK();
		}		

	return receive;
}


uint8_t IIC_WaitACK(void)          //等待应答
{
	IIC_SDA_IN();
	
	IIC_SDA_H;                    //默认SDA是高电平，如果后面变成低电平，判定应答成功
	
	Delay_us(1);
	
	IIC_SCL_H;
	
	Delay_us(1);
	
	//设置超时应答时间
	uint16_t overtime = 0;
	while(READ_SDA)
	{
		overtime++;
		if(overtime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_L;
	return 0;
}
