#include "public.h"

//计时器每1ms进一次中断
void TIM3_IntInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);				//使能 TIM3 时钟
	
	/*
	APB1 42Mhz TIM3=84Mhz
	84M/84=1M   1/1000000*1000= 1/1000 = 1ms
	
	*/
	
	TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 					//自动重装载值 84M/84 = 1M/1000 = 1K = 1ms
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1; 					//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	
	// 初始化定时器 TIM3
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 						//允许定时器 3 更新中断
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 					//定时器 3 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; 		//抢占优先级 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; 			//响应优先级 2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);									//初始化 NVIC
	TIM_Cmd(TIM3,ENABLE); 											//使能定时器 3
	
}


