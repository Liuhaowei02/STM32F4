#include "public.h"

/**
  * @brief  ADC1�����GPIO����  �������ģ��
  * @note   ������ADC�����ΪADC1_IN5  PA5

  */
static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AN; //ģ������ģʽ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; //����������

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}




/**
  * @brief  ADCģʽ���ú���
  * @note   ��Ҫ����ADC_CommonInitTypeDef��ADC_InitTypeDef�������ṹ��
  * @param  void
  * @retval None
  */
static void ADC_Mode_Config(void)
{
	ADC_CommonInitTypeDef  ADC_CommonInitStructure;
	ADC_InitTypeDef		   ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	/******************ADC Common�ṹ������***********************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//AP2 84Mhz , ADC���Ƶ��36Mhz ���������ǲ���ѡ2��Ƶ����������ѡ4��Ƶ 84Mhz/4=21Mhz
	ADC_CommonInitStructure.ADC_Prescaler= ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_20Cycles;//���λ�ϲɼ��ӳ����ڣ���Ϊ�����Ƕ���ģʽ������������ò�Ӱ�졣
	
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	/******************ADC Init�ṹ������***********************************/
	ADC_InitStructure.ADC_Resolution=ADC_Resolution_12b; //�ֱ���
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//����ת��
	//�ⲿ����ͨ��������ʹ��������������Դ�ֵ��㸳�費Ӱ��
	ADC_InitStructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion=1; //ָ��ת��ͨ������
	
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_56Cycles); 
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	
	//ʹ��ADC
	ADC_Cmd(ADC1, ENABLE);
	
	
	
	/* Start ADC1 Software Conversion */
	//��ʼADCת�����������
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
		//���ж�

}



void ADC_Init_Config(void)
{
	//1.��ʼ������ADC Ŀ������Ϊģ������ģʽ��
	ADC_GPIO_Config();
	
	//2.ADC-MODE
	ADC_Mode_Config();
	
	//3.NVIC
	ADC_NVIC_Config();
	
	ADC_SoftwareStartConv(ADC1);
}
