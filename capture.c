#include "public.h"

void TIMx_Capture_Config(void)
{ 
	TIMx_GPIO_Config();
	TIMx_NVIC_Config();

	//����TIM8ʱ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	//APB2 84Mhz  TIM8=168Mhz
	TIM_TimeBaseStructure.TIM_Period= 50000;  //ARR�������ֵ  ���벶����ʲôӰ�죿
	TIM_TimeBaseStructure.TIM_Prescaler= 168-1;//PSC��Ƶϵ��
	/*
		168Mhz/168=1Mhz ��һ������1/1000000 s  1/1Mhz =  168/168M x
	*/
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	
	//���벶������
	
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  //���벶��ͨ��ѡ��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	//������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//���ò���ͨ�����ź�ֱ���ͷ�ֱ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//1��Ƶ���������źŵ�ÿ����Ч���ض�����
	TIM_ICInitStructure.TIM_ICFilter = 0x0;//�˲������������������䣬��Ϊ�����ȶ���

	TIM_PWMIConfig(TIM8, &TIM_ICInitStructure);


	/* Select the TIM8 Input Trigger: TI1FP1 */
	TIM_SelectInputTrigger(TIM8, TIM_TS_TI1FP1);
	
	TIM_SelectSlaveMode(TIM8, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM8,TIM_MasterSlaveMode_Enable);
	
	TIM_Cmd(TIM8, ENABLE);
	
	TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);
	TIM_ITConfig(TIM8, TIM_IT_CC1, ENABLE);
}

static void TIMx_GPIO_Config(void)
{
	//1.TIM8 PC6 ���ó����벶��ͨ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* TIM4 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	/* GPIOB clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	/* TIM8 chenne1 configuration : PC.6 */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* Connect PC.6 pin to TIM8 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
}

static void TIMx_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


