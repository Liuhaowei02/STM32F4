#include "timer.h"
#include "stm32f4xx.h"

/**
  * @brief  ��ʱ��6��ʼ������
  * @note   ��ʱ��6������APB1ʱ�����ϣ����Զ�ʱ��ʱ��Ƶ��Ϊ42*2=84Mhz
  * @param  void 
  * @retval None
  */
void TIM6_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//3. ����������ʱ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	//4.���ö�ʱ�����ں�Ԥ��Ƶֵ��Ŀ�ļ���1s
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=50000-1;              //����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler=8400-1;
	/*
	a. APB1����ʱ��Ϊ42Mhz����ʱ��6ʱ��Ϊ42Mhz*2=84Mhz��84Mhz/8400=84000000/8400=10000hz  ��һ���� 1/10000=0.0001s=0.1ms
	b. ��10000hzƵ���£���10000��������1s
	*/
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	
	//5. ������ʱ���ĸ����ж�
	 TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);
	
	//6. ���ö�ʱ���ж�
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	//7. ������ʱ��
	TIM_Cmd(TIM6, ENABLE);	
	
}

