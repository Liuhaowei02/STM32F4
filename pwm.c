#include "public.h"


//一共用到两个PWM   
//一个给风扇 可以实现风扇不同转速
//一个给LED 可以实现LED的不同光强

//LED(名字用的FAN)配置 PA3->TIM5_CH4


static void PWM_FAN_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;   //GPIO结构体变量
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);    //使能GPIOA时钟

	//配置GPIOF6参数
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;							//指定复用模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;						//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;									//指定pin3管脚
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;					//浮空
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_100MHz;				//
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);                   //初始化GPIO外设
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5); //将PA3复用为TIM5_CH4
}

static void PWM_FAN_TIMER_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  					  //使能定时器5时钟
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       				  //定义定时器时基单元结构体变量
	TIM_OCInitTypeDef  TIM_OCInitStructure;                         //定义输出比较结构体变量
	
	//配置定时器14时基单元结构体变量成员参数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数
	TIM_TimeBaseStructure.TIM_Period =	1000-1	;                   //自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler =   168-1 ;                //预分频器值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //时钟分割：不分割
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);                 //初始化定时器5

	//配置输出比较结构体变量成员参数
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较模式PWM1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //极性高，不反转     
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                   //使能输出比较模式
	TIM_OCInitStructure.TIM_Pulse =     0x0;                        //CCR的值
	
	TIM_OC4Init(TIM5,&TIM_OCInitStructure);                        //初始化输出比较通道4

	//TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);              //开启影子寄存器（捕获/比较）
		
	TIM_Cmd(TIM5, ENABLE);                                         //使能计数器	
}

/*
			风扇配置  用的名字LED
      PF7->TIM11_CH1
*/
static void PWM_LED_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;   //GPIO结构体变量
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);    //使能GPIOF时钟
	
	
	//配置GPIOF9参数
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;							//指定复用模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;						//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;									//指定pin7管脚
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;					//
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_100MHz;				//
	
	GPIO_Init(GPIOF,&GPIO_InitStructure);                   //初始化GPIO外设
	

	GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11); //将PF7复用为TIM11_CH1

}

static void PWM_LED_TIMER_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);  					//使能定时器11时钟
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       				  //定义定时器时基单元结构体变量
	TIM_OCInitTypeDef  TIM_OCInitStructure;                         //定义输出比较结构体变量
	
	//配置定时器14时基单元结构体变量成员参数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数
	TIM_TimeBaseStructure.TIM_Period =	1000-1	;                   //自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler =    168-1;                //预分频器值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //时钟分割：不分割
	
	TIM_TimeBaseInit(TIM11,&TIM_TimeBaseStructure);                 //初始化定时器11

	//配置输出比较结构体变量成员参数
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较模式PWM1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //极性高，不反转     
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                   //使能输出比较模式
	TIM_OCInitStructure.TIM_Pulse =     0x0;                        //CCR的值
	
	TIM_OC1Init(TIM11,&TIM_OCInitStructure);                        //初始化输出比较通道1

	TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);              //开启影子寄存器（捕获/比较）
	
	
	TIM_Cmd(TIM11, ENABLE);                                         //使能计数器	
}

void PWM_LED_Init(void)
{

  PWM_LED_TIMER_Config();
	PWM_LED_GPIO_Config();
	PWM_FAN_GPIO_Config();
	PWM_FAN_TIMER_Config();

}

