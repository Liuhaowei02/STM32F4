#include "public.h"

//���ù���BH1750�Ĵ�����
//Э��IIC  PB6->SCL    PB7->SDA
static void IIC_HARD_GPIO_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);  //ʹ��ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;                  //����GPIO�ṹ�����
	
	//����GPIO�ṹ�������Ա������PB6->SCL    PB7->SDA
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //����ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;          //��©���  
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;    //ָ������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //����
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);                 //��ʼ��GPIOB����
	
	//��PB6����ΪIIC��SCL    PB7����ΪIIC��SDA
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C1);
}

static void IIC_Init_Config(void)
{
		I2C_InitTypeDef IIC_InitStructure;                  //����IIC�ṹ�����
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE); //ʹ��IIC1ʱ��
		
		//����IIC�ṹ�������Ա����
		IIC_InitStructure.I2C_Ack = I2C_Ack_Enable;         //ʹ��Ӧ��λ
		IIC_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit; //Ӧ���ַ7λ
		IIC_InitStructure.I2C_ClockSpeed = 100000;          //ʱ��Ƶ��100k
		IIC_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  //ռ�ձ�1/3  ��/��=2/1 
		IIC_InitStructure.I2C_Mode = I2C_Mode_I2C;          //IICģʽ
		IIC_InitStructure.I2C_OwnAddress1 = 0x0;            //�����ַ�����漰
		
		I2C_Init(I2C1,&IIC_InitStructure);                  //��ʼ��IIC1
		
		I2C_Cmd(I2C1,ENABLE);																//ʹ��IIC1
}
	
void IIC1_Init(void)
{
		IIC_HARD_GPIO_Config();
		IIC_Init_Config();
}
	
