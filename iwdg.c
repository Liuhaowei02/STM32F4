#include "public.h"

void IWDG_Init_Config(void)
{
	//取消写保护
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//设置分频系统
	IWDG_SetPrescaler(IWDG_Prescaler_32); 
	//32分频  32khz/32=1khz  倒数一个数：1/1khz = 1/1000hz =0.001s = 1ms
	
	//设置重装载值
	IWDG_SetReload(2000);  //数2000个数，用时2s
	IWDG_ReloadCounter();
	
	//启动
	IWDG_Enable();

}

