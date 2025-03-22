#include "public.h"

/**
  * @brief  �ⲿ�жϳ�ʼ������
  * @note   ����PE2��KEY2����Ϊ�ж�Դ
  * @param  void 
  * @retval None
  */
void EXTI_Init_Key(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;  //�����ⲿ�жϵĽṹ��
	NVIC_InitTypeDef   NVIC_InitStructure;  //����NVIC�Ľṹ��
	
	//a.����SYSCFG�����Ӧ��ʱ��
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	
	//b.ѡ���Ӧ���ⲿ�ж��ߣ�������PE2��KEY2�����뵽�ж���·������
	/* Connect EXTI Line2 to PE2 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);

	//c.EXTI�ж�����
   /* Configure EXTI Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_Init(&EXTI_InitStructure);

	//d. NVIC����
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
