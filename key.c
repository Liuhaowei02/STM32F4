#include "public.h"



void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN; //输入模式
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN; //下拉模式
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed; //引脚响应速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN; //输入模式
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP; //上拉模式
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed; //引脚响应速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
}

bool GetStaKey(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)//被按下了
	{
		//消除前抖动
		Delay_ms(10);
		
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1) ;
		
		//消除后抖动
		Delay_ms(10);
		return true;	
	}
	else 
		return false;
	
}	



uint8_t KEY_Scan(void)
{
	static  uint8_t key_up=1;  //按键松开按下的标记位，1没有按键被按下，0有按键被按下
	
	if(key_up&&(WK_UP==1||KEY0==0||KEY1==0||KEY2==0))	 //有任何一个按键被按下
	{ 
		key_up=0;
		
		Delay_ms(10);
		
		if(KEY0==0)
			return 1;
		else if(KEY1==0)
			return 2;
		else if(KEY2==0)
			return 3;
		else if(WK_UP==1)
			return 4;
	}else if(WK_UP==0&&KEY0==1&&KEY1==1&&KEY2==1) //没有任何按键被按下
			key_up=1;
	
	
	
		return 0;
	
}









