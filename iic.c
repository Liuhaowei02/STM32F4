#include "public.h"
//����ģ��IIC����
//���ģ��IIC  PE5->SCL, PE6->SDA
void IIC_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_IIC_SCL,ENABLE);   //ʹ��SCL����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_IIC_SDA,ENABLE);   //ʹ��SDA����ʱ��
	
	//����SCL����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;      //���ģʽ 
	GPIO_InitStructure.GPIO_Pin=IIC_SCL;					   //PE5->SCL
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			 //����ģʽ	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;     //��©���
	
	GPIO_Init(IIC_SCL_PORT,&GPIO_InitStructure);     //��ʼ��SCL����
	
	//����SDA����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;      //���ģʽ 
	GPIO_InitStructure.GPIO_Pin= IIC_SDA;   				 //PE6->SDA
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			 //����ģʽ	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;     //��©���
	
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);     //��ʼ��SDA����
	
	//Ĭ��SCL��SDA�ߵ�ƽ
	IIC_SDA_H;
	IIC_SCL_H;
	
}


static void IIC_SDA_OUT(void) 														//����SDAΪ���ģʽ���������ݣ�д��
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	//����SDA����Ϊ���ģʽ
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;      //���ģʽ 
	GPIO_InitStructure.GPIO_Pin= IIC_SDA;   				 //PE6->SDA
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			 //����ģʽ	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;     //��©���
	
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);     //��ʼ��SDA����	
	
}


static void IIC_SDA_IN(void)															//����SDAΪ����ģʽ���������ݣ�����
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	//����SDA����Ϊ���ģʽ
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;      //����ģʽ 
	GPIO_InitStructure.GPIO_Pin= IIC_SDA;   				 //PE6->SDA
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			 //����ģʽ	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);     //��ʼ��SDA����	
	
}


void IIC_Start(void)           //IIC��ʼ�ź�
{
	IIC_SDA_OUT();

	IIC_SCL_H;                   //����SCL��
	IIC_SDA_H;                   //����SDA��
	Delay_us(4);                 //�øߵ�ƽ����ƽ��
	
	IIC_SDA_L;                   //����SCL�ߵ�ƽ״̬������SDA,�����½��أ���ʼ�ź�
	
	Delay_us(4);
	
	IIC_SCL_L;									 //׼����������
}


void IIC_Stop(void)						 //IICֹͣ�ź�
{
	IIC_SDA_OUT();
	

	IIC_SCL_H;                   //����SCL��
	IIC_SDA_L;                   //����SDA��	
	Delay_us(4);
	
	IIC_SDA_H;									 //����SDA�����������أ������ź�
	
}


void IIC_ACK(void)             //IICӦ���ź�
{
	IIC_SDA_OUT();
	IIC_SCL_L;									 //SCLΪ�͵�ƽ״̬�£�SDA��ʼ׼������
	IIC_SDA_L;                   //����SDA�ߣ��ӻ�׼��Ӧ��λ��0����ʾ���ճɹ�
	
	Delay_us(1);
	
	IIC_SCL_H;                   //SCL�ߵ�ƽ,Ӧ��λ��Ч,�����0��ʾ���ܳɹ�
	Delay_us(1);
	
	IIC_SCL_L;                   //һ�����ڽ������ȴ��¸���������׼��
	
}

void IIC_NACK(void)
{
	IIC_SDA_OUT();
	IIC_SCL_L;									 //SCLΪ�͵�ƽ״̬�£�SDA��ʼ׼������
	IIC_SDA_H;                   //����SDA�ߣ��ӻ�׼��Ӧ��λ��1����ʾ����ʧ��
	
	Delay_us(1);
	
	IIC_SCL_H;                   //SCL�ߵ�ƽ,Ӧ��λ��Ч,�����1��ʾ����ʧ��
	Delay_us(1);	
	
	IIC_SCL_L;                   //һ�����ڽ������ȴ��¸���������׼��
}


void IIC_SendByte(uint8_t data)        //����һ���ֽ�����
{
	IIC_SDA_OUT();
	IIC_SCL_L;                           //��SCLΪ�͵�ƽ�£�SDA׼������ 
	
	uint8_t i=0;
	for(i=0;i<8;i++)
	{
		if(data &0x80)                    //�жϵ�һλ�ǲ���1
		{
			IIC_SDA_H;	                    //����SDA
		}
		else
		{
			IIC_SDA_L;                      //SDA�͵�ƽ
		}
		IIC_SCL_H;                        //����SCL,������Ч
		Delay_us(1);
		
		IIC_SCL_L;                        //����SCL,�ȴ�SDA׼����һλ����
		Delay_us(1);
		
		data<<=1;                     //��λ
	}
}


uint8_t IIC_ReadByte(uint8_t ack)       //����һ���ֽ�
{
	IIC_SDA_IN();
	
	uint8_t i=0;
	uint8_t receive=0;
	for(i=0;i<8;i++)
	{	
		IIC_SCL_L;											//SCL���ͣ��ȴ��ӻ�׼������
		Delay_us(1);
		IIC_SCL_H;                      //����SCL��������Ч
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


uint8_t IIC_WaitACK(void)          //�ȴ�Ӧ��
{
	IIC_SDA_IN();
	
	IIC_SDA_H;                    //Ĭ��SDA�Ǹߵ�ƽ����������ɵ͵�ƽ���ж�Ӧ��ɹ�
	
	Delay_us(1);
	
	IIC_SCL_H;
	
	Delay_us(1);
	
	//���ó�ʱӦ��ʱ��
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
