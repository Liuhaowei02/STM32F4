#include "public.h"


void WWDG_Init_Config(void)
{
	// 开启窗口看门狗时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	//设置预分频系数
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	//计数器计数频率=42Mhz/4096 / 8 = 1281hz    数一个数：1/1281hz= 0.00078s = 780us
	
	//设置上窗口值
	WWDG_SetWindowValue(80); //上窗口最大值 0x7F(127)
	
	/* 780 * (127-80) = 36.6ms < 刷新窗口 <  780 * (127-64+1) = 49.9ms*/

	//设置看门狗计数器的值，并且使能
	WWDG_Enable(127);
		
	
	//开中断之前，先清除中断标志位
	WWDG_ClearFlag();
	
	//配置NVIC
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
	
	//开中断
	WWDG_EnableIT();
	
	

}

