#include "public.h"

//配置光敏BH1750的传输线
//协议IIC  PB6->SCL    PB7->SDA
static void IIC_HARD_GPIO_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);  //使能时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;                  //定义GPIO结构体变量
	
	//配置GPIO结构体变量成员参数：PB6->SCL    PB7->SDA
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //复用模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;          //开漏输出  
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;    //指定引脚
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //上拉
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);                 //初始化GPIOB外设
	
	//将PB6复用为IIC的SCL    PB7复用为IIC的SDA
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C1);
}

static void IIC_Init_Config(void)
{
		I2C_InitTypeDef IIC_InitStructure;                  //定义IIC结构体变量
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE); //使能IIC1时钟
		
		//配置IIC结构体变量成员参数
		IIC_InitStructure.I2C_Ack = I2C_Ack_Enable;         //使能应答位
		IIC_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit; //应答地址7位
		IIC_InitStructure.I2C_ClockSpeed = 100000;          //时钟频率100k
		IIC_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  //占空比1/3  低/高=2/1 
		IIC_InitStructure.I2C_Mode = I2C_Mode_I2C;          //IIC模式
		IIC_InitStructure.I2C_OwnAddress1 = 0x0;            //自身地址：不涉及
		
		I2C_Init(I2C1,&IIC_InitStructure);                  //初始化IIC1
		
		I2C_Cmd(I2C1,ENABLE);																//使能IIC1
}
	
void IIC1_Init(void)
{
		IIC_HARD_GPIO_Config();
		IIC_Init_Config();
}
	
