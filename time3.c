#include "public.h"

//��ʱ��ÿ1ms��һ���ж�
void TIM3_IntInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);				//ʹ�� TIM3 ʱ��
	
	/*
	APB1 42Mhz TIM3=84Mhz
	84M/84=1M   1/1000000*1000= 1/1000 = 1ms
	
	*/
	
	TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 					//�Զ���װ��ֵ 84M/84 = 1M/1000 = 1K = 1ms
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1; 					//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	
	// ��ʼ����ʱ�� TIM3
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 						//����ʱ�� 3 �����ж�
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 					//��ʱ�� 3 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; 		//��ռ���ȼ� 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; 			//��Ӧ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);									//��ʼ�� NVIC
	TIM_Cmd(TIM3,ENABLE); 											//ʹ�ܶ�ʱ�� 3
	
}


