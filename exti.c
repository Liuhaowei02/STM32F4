#include "public.h"

/**
  * @brief  外部中断初始化函数
  * @note   按键PE2（KEY2）作为中断源
  * @param  void 
  * @retval None
  */
void EXTI_Init_Key(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;  //配置外部中断的结构体
	NVIC_InitTypeDef   NVIC_InitStructure;  //配置NVIC的结构体
	
	//a.开启SYSCFG外设对应的时钟
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	
	//b.选择对应的外部中断线，将按键PE2（KEY2）接入到中断线路中来。
	/* Connect EXTI Line2 to PE2 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);

	//c.EXTI中断配置
   /* Configure EXTI Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_Init(&EXTI_InitStructure);

	//d. NVIC配置
	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
	NVIC_Init(&NVIC_InitStructure);	
}
