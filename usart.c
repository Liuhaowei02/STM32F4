#include "public.h"
#include <stdarg.h>
#include <string.h>


//��������UASRT   UASRT1  PA9TX   PA10RX
void USART_Init_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//1.ʹ��RX ��TX ����GPIO(PA9 PA10)ʱ�Ӻ�USART1 ʱ�ӣ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//PA9 PA10 GPIO���ų�ʼ�������óɸ���ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP; //����ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed; //������Ӧ�ٶ�
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//2.��ʼ��GPIO������PA9��PA10 ���õ�USART �ϣ�
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//3.����USART����
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





//����һ���ַ�,���͵����ַ�ǰ������ʹ��
void Usart_SendByte(USART_TypeDef * pUSARTx , uint16_t ch)
{

	
	//����һ���ֽ����ݵ�USARTx
	USART_SendData(pUSARTx,ch);
	
	//�ȴ��������ݼĴ���Ϊ��,TXE λ��Ӳ���� 1��
	//����ʾ��USART_DR (���ݼĴ���)�п�д����һ�����ݣ������Ḳ��ǰһ�����ݡ�
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);

}



//�����ַ���
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
	USART_SendData(USART1, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		Serial_SendByte(String[i]);		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
}


void Serial_Printf(char *format, ...)
{
	char String[100];				//�����ַ�����
	va_list arg;					//����ɱ�����б��������͵ı���arg
	va_start(arg, format);			//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);	//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);					//��������arg
	Serial_SendString(String);		//���ڷ����ַ����飨�ַ�����
}

