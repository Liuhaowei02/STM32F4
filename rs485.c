#include "public.h"




//对串口2引脚初始化，用于RS485通讯
static void RS485_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP; //上拉模式
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed; //引脚响应速度

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//2.初始化GPIO，并将PA2和PA3 复用到USART2 上；
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	//配置PG8引脚，控制收发使能
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP; //上拉模式
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed; //引脚响应速度

	GPIO_Init(GPIOG, &GPIO_InitStructure);
}


//串口2配置
static void USARTX_Init_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART2,&USART_InitStructure);
	
	//使能串口1
	USART_Cmd(USART2,ENABLE);
	
	//使能接受中断
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
}



static void RS485_NVIC_Config(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}


void RS485_Init_Config(void)
{
	//1. 初始化RS485 通讯使用的USART2 外设及相关引脚
	RS485_GPIO_Config();
	
	
	
	//2. 串口2配置
	USARTX_Init_Config();
	
	
	
	//3. 中断优先级配置
	RS485_NVIC_Config();
	
	
	//4. 默认处于接收模式
	GPIO_ResetBits(GPIOG,GPIO_Pin_8); //低电平，接收使能

}


//发送一个字符,发送单个字符前，发送使能
void RS485_SendByte( uint8_t ch)
{
	
	//发送一个字节数据到USART2
	USART_SendData(USART2,ch);
	
	//等待发送数据寄存器为空,TXE 位由硬件置 1，
	//它表示：USART_DR (数据寄存器)中可写入下一个数据，而不会覆盖前一个数据。
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
}



//发送字符串
void RS485_SendString(char* str)
{
	uint16_t k = 0;
	
	//发送数据之前，要开启发送使能
	GPIO_SetBits(GPIOG,GPIO_Pin_8); //高电平，发送使能
	
	do
	{
		RS485_SendByte(*(str+k));
		k++;
	}while(*(str+k)!='\0');
	
	//加一个短暂的延时，确保485数据发送完毕,也不是必须的，只是有这个概率而已
	Delay_ms(2);
	
	//发送数据之后，要立马开启接收使能
	GPIO_ResetBits(GPIOG,GPIO_Pin_8); 	
}





