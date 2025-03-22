#include "public.h"
#include <stdarg.h>
#include <string.h>


//配置蓝牙UASRT   UASRT1  PA9TX   PA10RX
void USART_Init_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//1.使能RX 和TX 引脚GPIO(PA9 PA10)时钟和USART1 时钟；
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//PA9 PA10 GPIO引脚初始化，配置成复用模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP; //上拉模式
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed; //引脚响应速度
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//2.初始化GPIO，并将PA9和PA10 复用到USART 上；
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//3.配置USART参数
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1,&USART_InitStructure);

	USART_Cmd(USART1,ENABLE);
	
}

/*


*/





//发送一个字符,发送单个字符前，发送使能
void Usart_SendByte(USART_TypeDef * pUSARTx , uint16_t ch)
{

	
	//发送一个字节数据到USARTx
	USART_SendData(pUSARTx,ch);
	
	//等待发送数据寄存器为空,TXE 位由硬件置 1，
	//它表示：USART_DR (数据寄存器)中可写入下一个数据，而不会覆盖前一个数据。
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);

}



//发送字符串
void Usart_SendString(USART_TypeDef * pUSARTx , char* str)
{
	uint16_t k = 0;
	
	do
	{
		Usart_SendByte(pUSARTx,*(str+k));
		k++;
	}while(*(str+k)!='\0');
	
}




void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Serial_SendByte(String[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}


void Serial_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	Serial_SendString(String);		//串口发送字符数组（字符串）
}

