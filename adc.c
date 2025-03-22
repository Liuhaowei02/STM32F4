#include "public.h"

/**
  * @brief  ADC1输入口GPIO配置  土壤检测模块
  * @note   本例程ADC输入口为ADC1_IN5  PA5

  */
static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AN; //模拟输入模式
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; //不上拉下拉

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}




/**
  * @brief  ADC模式配置函数
  * @note   主要配置ADC_CommonInitTypeDef和ADC_InitTypeDef这两个结构体
  * @param  void
  * @retval None
  */
static void ADC_Mode_Config(void)
{
	ADC_CommonInitTypeDef  ADC_CommonInitStructure;
	ADC_InitTypeDef		   ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	/******************ADC Common结构体配置***********************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//AP2 84Mhz , ADC最大频率36Mhz ，所以我们不能选2分频，这里我们选4分频 84Mhz/4=21Mhz
	ADC_CommonInitStructure.ADC_Prescaler= ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_20Cycles;//两次混合采集延迟周期，因为我们是独立模式，这里随便设置不影响。
	
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	/******************ADC Init结构体配置***********************************/
	ADC_InitStructure.ADC_Resolution=ADC_Resolution_12b; //分辨率
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//连续转换
	//外部触发通道，本例使用软件触发，所以此值随便赋予不影响
	ADC_InitStructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion=1; //指定转换通道个数
	
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_56Cycles); 
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	
	//使能ADC
	ADC_Cmd(ADC1, ENABLE);
	
	
	
	/* Start ADC1 Software Conversion */
	//开始ADC转换，软件触发
//
}


/**
  * @brief  ADC End of conversion interrupt
  * @note  
  * @param  void 
  * @retval None
  */
static void ADC_NVIC_Config(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
		//开中断

}



void ADC_Init_Config(void)
{
	//1.初始化配置ADC 目标引脚为模拟输入模式；
	ADC_GPIO_Config();
	
	//2.ADC-MODE
	ADC_Mode_Config();
	
	//3.NVIC
	ADC_NVIC_Config();
	
	ADC_SoftwareStartConv(ADC1);
}
