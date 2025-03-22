#include "public.h"




//�Դ���2���ų�ʼ��������RS485ͨѶ
static void RS485_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP; //����ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed; //������Ӧ�ٶ�

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//2.��ʼ��GPIO������PA2��PA3 ���õ�USART2 �ϣ�
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	//����PG8���ţ������շ�ʹ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP; //����ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed; //������Ӧ�ٶ�

	GPIO_Init(GPIOG, &GPIO_InitStructure);
}


//����2����
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
	
	//ʹ�ܴ���1
	USART_Cmd(USART2,ENABLE);
	
	//ʹ�ܽ����ж�
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
	//1. ��ʼ��RS485 ͨѶʹ�õ�USART2 ���輰�������
	RS485_GPIO_Config();
	
	
	
	//2. ����2����
	USARTX_Init_Config();
	
	
	
	//3. �ж����ȼ�����
	RS485_NVIC_Config();
	
	
	//4. Ĭ�ϴ��ڽ���ģʽ
	GPIO_ResetBits(GPIOG,GPIO_Pin_8); //�͵�ƽ������ʹ��

}


//����һ���ַ�,���͵����ַ�ǰ������ʹ��
void RS485_SendByte( uint8_t ch)
{
	
	//����һ���ֽ����ݵ�USART2
	USART_SendData(USART2,ch);
	
	//�ȴ��������ݼĴ���Ϊ��,TXE λ��Ӳ���� 1��
	//����ʾ��USART_DR (���ݼĴ���)�п�д����һ�����ݣ������Ḳ��ǰһ�����ݡ�
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
}



//�����ַ���
void RS485_SendString(char* str)
{
	uint16_t k = 0;
	
	//��������֮ǰ��Ҫ��������ʹ��
	GPIO_SetBits(GPIOG,GPIO_Pin_8); //�ߵ�ƽ������ʹ��
	
	do
	{
		RS485_SendByte(*(str+k));
		k++;
	}while(*(str+k)!='\0');
	
	//��һ�����ݵ���ʱ��ȷ��485���ݷ������,Ҳ���Ǳ���ģ�ֻ����������ʶ���
	Delay_ms(2);
	
	//��������֮��Ҫ����������ʹ��
	GPIO_ResetBits(GPIOG,GPIO_Pin_8); 	
}





