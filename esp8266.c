#include "public.h"
#include <stdarg.h>
#include <string.h>

//WIFI	ռ������PB10->TX  PB11->RX  PF6->GBC KEY  PC0->GBC LED
unsigned char esp8266_buf[256];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;

static void USART3_GPIO_Config(void)
{
	/* ����GPIOB��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* ����GPIO�ṹ�� */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ����GPIO���� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;        /*!< ѡ���10�ź͵�11������ */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    /*!< ��������Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;									/*!< ��������Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                    /*!< ��������Ϊ�������루����ֵ�� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;                /*!< ����GPIO�ٶ�Ϊ���٣�50MHz�� */
	
	/* ��ʼ��GPIOB */
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);        /*!< PB10����USART3_Tx��ע��F407��F103����һ���� */
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);        /*!< PB11����USART3_Rx��ע��F407��F103����һ���� */
	
}

/**
 * @brief ��ʼ��USART3��USART����
 */
static void USART3_USART_Config(void)
{
	/* ����USART3��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	/* ����USART�ṹ�� */
	USART_InitTypeDef USART_InitStructure;
	
	/* ����USART�ṹ�� */
	USART_InitStructure.USART_BaudRate = 115200;                                     /*!< ���ò�����Ϊ�����ֵ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                       /*!< 8λ�ֽڳ��� */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                            /*!< 1λֹͣλ */
	USART_InitStructure.USART_Parity = USART_Parity_No;                               /*!< ��У��λ */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   /*!< ��Ӳ�������� */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                   /*!< �շ�ģʽ */
	
	/* ��ʼ��USART3 */
	USART_Init(USART3,&USART_InitStructure);
	
	/* ʹ��USART3 */
	USART_Cmd(USART3,ENABLE);
}


void USART3WIFI_Init_Config(void)
{
	USART3_GPIO_Config();
	USART3_USART_Config();
}


/*
		ESP8266��ʼ��
		����ATָ����Ҫʱ�䣬������ʱ
*/
void ESP8266_Init(void)
{	

	printf("AT+RST\r\n");           //����
	Delay_ms(200);
	printf("ATE0\r\n");             //�رջ���
	Delay_ms(200);
	printf("AT+CWMODE=3\r\n");      //����˫ģʽ����
	Delay_ms(200);
	printf("AT+CWJAP=\"SGYC\",\"sgyc2023\"\r\n");       //����WiF  WiF����SGYC   ���룺sgyc2023
	Delay_ms(8000);
	printf("AT+MQTTUSERCFG=0,1,\"NULL\",\"test2&k2266VGrZMU\",\"e4ef7ce91e9dc1877dd29a10d3af90d15f68c6accd5773e14743af11b54fdda1\",0,0,\"\"\r\n");   //������ƽ̨
	Delay_ms(200);
	printf("AT+MQTTCLIENTID=0,\"k2266VGrZMU.test2|securemode=2\\,signmethod=hmacsha256\\,timestamp=1732928371237|\"\r\n");         
	Delay_ms(200);
	printf("AT+MQTTCONN=0,\"iot-06z00g1sqr4if93.mqtt.iothub.aliyuncs.com\",1883,1\r\n");    //������
	Delay_ms(200);
	
}


//�ض���c�⺯��printf������1���������ǾͿ���ʹ��printf������ӡ�����Ϣ������1��
int fputc(int ch, FILE *stream)
{
	//����һ���ֽ����ݵ�����1
	USART_SendData(USART3,(uint16_t)ch);
	
	//�ȴ��������ݼĴ���Ϊ��
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);

	return ch;
}