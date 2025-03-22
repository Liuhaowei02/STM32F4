#include "timer.h"
#include "stm32f4xx.h"

/**
  * @brief  定时器6初始化函数
  * @note   定时器6挂载在APB1时钟线上，所以定时器时钟频率为42*2=84Mhz
  * @param  void 
  * @retval None
  */
void TIM6_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//3. 开启基本定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	//4.设置定时器周期和预分频值，目的计数1s
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=50000-1;              //计数值
	TIM_TimeBaseStructure.TIM_Prescaler=8400-1;
	/*
	a. APB1总线时钟为42Mhz，定时器6时钟为42Mhz*2=84Mhz，84Mhz/8400=84000000/8400=10000hz  数一个数 1/10000=0.0001s=0.1ms
	b. 在10000hz频率下，数10000个数就是1s
	*/
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	
	//5. 启动定时器的更新中断
	 TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);
	
	//6. 配置定时器中断
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	//7. 开启定时器
	TIM_Cmd(TIM6, ENABLE);	
	
}

