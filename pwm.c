#include "public.h"


//һ���õ�����PWM   
//һ�������� ����ʵ�ַ��Ȳ�ͬת��
//һ����LED ����ʵ��LED�Ĳ�ͬ��ǿ

//LED(�����õ�FAN)���� PA3->TIM5_CH4


static void PWM_FAN_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;   //GPIO�ṹ�����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);    //ʹ��GPIOAʱ��

	//����GPIOF6����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;							//ָ������ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;						//�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;									//ָ��pin3�ܽ�
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;					//����
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_100MHz;				//
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);                   //��ʼ��GPIO����
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5); //��PA3����ΪTIM5_CH4
}

static void PWM_FAN_TIMER_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  					  //ʹ�ܶ�ʱ��5ʱ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       				  //���嶨ʱ��ʱ����Ԫ�ṹ�����
	TIM_OCInitTypeDef  TIM_OCInitStructure;                         //��������ȽϽṹ�����
	
	//���ö�ʱ��14ʱ����Ԫ�ṹ�������Ա����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //���ϼ���
	TIM_TimeBaseStructure.TIM_Period =	1000-1	;                   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =   168-1 ;                //Ԥ��Ƶ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //ʱ�ӷָ���ָ�
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);                 //��ʼ����ʱ��5

	//��������ȽϽṹ�������Ա����
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //����Ƚ�ģʽPWM1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //���Ըߣ�����ת     
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                   //ʹ������Ƚ�ģʽ
	TIM_OCInitStructure.TIM_Pulse =     0x0;                        //CCR��ֵ
	
	TIM_OC4Init(TIM5,&TIM_OCInitStructure);                        //��ʼ������Ƚ�ͨ��4

	//TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);              //����Ӱ�ӼĴ���������/�Ƚϣ�
		
	TIM_Cmd(TIM5, ENABLE);                                         //ʹ�ܼ�����	
}

/*
			��������  �õ�����LED
      PF7->TIM11_CH1
*/
static void PWM_LED_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;   //GPIO�ṹ�����
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);    //ʹ��GPIOFʱ��
	
	
	//����GPIOF9����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;							//ָ������ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;						//�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;									//ָ��pin7�ܽ�
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;					//
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_100MHz;				//
	
	GPIO_Init(GPIOF,&GPIO_InitStructure);                   //��ʼ��GPIO����
	

	GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11); //��PF7����ΪTIM11_CH1

}

static void PWM_LED_TIMER_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);  					//ʹ�ܶ�ʱ��11ʱ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       				  //���嶨ʱ��ʱ����Ԫ�ṹ�����
	TIM_OCInitTypeDef  TIM_OCInitStructure;                         //��������ȽϽṹ�����
	
	//���ö�ʱ��14ʱ����Ԫ�ṹ�������Ա����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //���ϼ���
	TIM_TimeBaseStructure.TIM_Period =	1000-1	;                   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =    168-1;                //Ԥ��Ƶ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //ʱ�ӷָ���ָ�
	
	TIM_TimeBaseInit(TIM11,&TIM_TimeBaseStructure);                 //��ʼ����ʱ��11

	//��������ȽϽṹ�������Ա����
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //����Ƚ�ģʽPWM1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //���Ըߣ�����ת     
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                   //ʹ������Ƚ�ģʽ
	TIM_OCInitStructure.TIM_Pulse =     0x0;                        //CCR��ֵ
	
	TIM_OC1Init(TIM11,&TIM_OCInitStructure);                        //��ʼ������Ƚ�ͨ��1

	TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);              //����Ӱ�ӼĴ���������/�Ƚϣ�
	
	
	TIM_Cmd(TIM11, ENABLE);                                         //ʹ�ܼ�����	
}

void PWM_LED_Init(void)
{

  PWM_LED_TIMER_Config();
	PWM_LED_GPIO_Config();
	PWM_FAN_GPIO_Config();
	PWM_FAN_TIMER_Config();

}

