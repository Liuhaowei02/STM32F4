#include "public.h"
#include <stdarg.h>
#include <string.h>

//WIFI	占用引脚PB10->TX  PB11->RX  PF6->GBC KEY  PC0->GBC LED
unsigned char esp8266_buf[256];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;

static void USART3_GPIO_Config(void)
{
	/* 开启GPIOB的时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* 声明GPIO结构体 */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 配置GPIO引脚 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;        /*!< 选择第10号和第11号引脚 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    /*!< 设置引脚为复用模式 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;									/*!< 设置引脚为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                    /*!< 设置引脚为浮空输入（任意值） */
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;                /*!< 设置GPIO速度为快速（50MHz） */
	
	/* 初始化GPIOB */
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);        /*!< PB10复用USART3_Tx（注：F407比F103多这一步） */
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);        /*!< PB11复用USART3_Rx（注：F407比F103多这一步） */
	
}

/**
 * @brief 初始化USART3的USART配置
 */
static void USART3_USART_Config(void)
{
	/* 开启USART3的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	/* 声明USART结构体 */
	USART_InitTypeDef USART_InitStructure;
	
	/* 配置USART结构体 */
	USART_InitStructure.USART_BaudRate = 115200;                                     /*!< 配置波特率为传入的值 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                       /*!< 8位字节长度 */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                            /*!< 1位停止位 */
	USART_InitStructure.USART_Parity = USART_Parity_No;                               /*!< 无校验位 */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   /*!< 无硬件流控制 */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                   /*!< 收发模式 */
	
	/* 初始化USART3 */
	USART_Init(USART3,&USART_InitStructure);
	
	/* 使能USART3 */
	USART_Cmd(USART3,ENABLE);
}


void USART3WIFI_Init_Config(void)
{
	USART3_GPIO_Config();
	USART3_USART_Config();
}


/*
		ESP8266初始化
		处理AT指令需要时间，设置延时
*/
void ESP8266_Init(void)
{	

	printf("AT+RST\r\n");           //重启
	Delay_ms(200);
	printf("ATE0\r\n");             //关闭回显
	Delay_ms(200);
	printf("AT+CWMODE=3\r\n");      //设置双模式可用
	Delay_ms(200);
	printf("AT+CWJAP=\"SGYC\",\"sgyc2023\"\r\n");       //连接WiF  WiF名：SGYC   密码：sgyc2023
	Delay_ms(8000);
	printf("AT+MQTTUSERCFG=0,1,\"NULL\",\"test2&k2266VGrZMU\",\"e4ef7ce91e9dc1877dd29a10d3af90d15f68c6accd5773e14743af11b54fdda1\",0,0,\"\"\r\n");   //连接云平台
	Delay_ms(200);
	printf("AT+MQTTCLIENTID=0,\"k2266VGrZMU.test2|securemode=2\\,signmethod=hmacsha256\\,timestamp=1732928371237|\"\r\n");         
	Delay_ms(200);
	printf("AT+MQTTCONN=0,\"iot-06z00g1sqr4if93.mqtt.iothub.aliyuncs.com\",1883,1\r\n");    //连接域
	Delay_ms(200);
	
}


//重定向c库函数printf到串口1，这样我们就可以使用printf函数打印输出信息到串口1了
int fputc(int ch, FILE *stream)
{
	//发送一个字节数据到串口1
	USART_SendData(USART3,(uint16_t)ch);
	
	//等待发送数据寄存器为空
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);

	return ch;
}