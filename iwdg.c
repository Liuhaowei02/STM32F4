#include "public.h"

void IWDG_Init_Config(void)
{
	//ȡ��д����
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//���÷�Ƶϵͳ
	IWDG_SetPrescaler(IWDG_Prescaler_32); 
	//32��Ƶ  32khz/32=1khz  ����һ������1/1khz = 1/1000hz =0.001s = 1ms
	
	//������װ��ֵ
	IWDG_SetReload(2000);  //��2000��������ʱ2s
	IWDG_ReloadCounter();
	
	//����
	IWDG_Enable();

}

