#include "public.h"


void WWDG_Init_Config(void)
{
	// �������ڿ��Ź�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	//����Ԥ��Ƶϵ��
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	//����������Ƶ��=42Mhz/4096 / 8 = 1281hz    ��һ������1/1281hz= 0.00078s = 780us
	
	//�����ϴ���ֵ
	WWDG_SetWindowValue(80); //�ϴ������ֵ 0x7F(127)
	
	/* 780 * (127-80) = 36.6ms < ˢ�´��� <  780 * (127-64+1) = 49.9ms*/

	//���ÿ��Ź���������ֵ������ʹ��
	WWDG_Enable(127);
		
	
	//���ж�֮ǰ��������жϱ�־λ
	WWDG_ClearFlag();
	
	//����NVIC
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
	
	//���ж�
	WWDG_EnableIT();
	
	

}

