#include "public.h"




void SysTick_Init(void)
{
	//ϵͳ��ʱ��Ƶ��Ϊ168Mhz  
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	
	/*��װ�ؼĴ�������configTICK_RATE_HZ�趨���ʱ�䣬
	�����ֵΪx��Ƶ��Ϊ168*1000000HZ���ж�����Ϊ1/configTICK_RATE_HZ
	
	x * 1/(168*1000000)=1/ configTICK_RATE_HZ
	x= 168*1000000/ configTICK_RATE_HZ
	
	��Ϊ24λ�Ĵ���,���ֵ:16777216����168M��,Լ��0.0998s���ң�
	*/
	SysTick->LOAD =(168*1000000/configTICK_RATE_HZ)-1;
	// �����ǰ����ֵ
    SysTick->VAL = 0; 
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//����SYSTICK�ж�
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //����SYSTICK 
	
}



/*
�� FreeRTOS ��ʹ�� SysTick ��Ϊ��ʱ������ҪС�ģ�
��Ϊ FreeRTOS �Լ�ʹ���� SysTick ��Ϊϵͳʱ�ӡ�
���ֱ�Ӳ��� SysTick ��ʱ�������ܻ���� FreeRTOS �ĵ��ȡ�
*/
void Delay_us(uint32_t  nTime)
{ 
	uint32_t ticks,tickcnt=0;
    uint32_t startTick, currentTick;
	
	// �� nTime ת��Ϊ�δ���
    ticks = (168000000 / 1000000) * nTime;
	
	// ��¼��ǰ�� SysTick ������ֵ
    startTick = SysTick->VAL;
	
	do
    {
        // ��ȡ��ǰ�� SysTick ������ֵ
        currentTick = SysTick->VAL;

        // ������ĵδ���,��Ϊ�ǵݼ�������
        if (startTick > currentTick)
        {
			tickcnt+=startTick - currentTick;
        }
        else
        {
			// ������ SysTick �����������
			tickcnt+=startTick + (SysTick->LOAD - currentTick);
        }
		
		startTick=currentTick;
		
		if(tickcnt>=ticks)
			break; //ʱ�䳬������Ҫ�ӳٵ�ʱ��,���˳�.
		
    } while (1);
	
}



void Delay_ms(uint32_t nTime)
{
	uint32_t ticks,tickcnt=0;
    uint32_t startTick, currentTick;
	
	// �� nTime ת��Ϊ�δ���
    ticks = (168000000 / 1000) * nTime;
	
	// ��¼��ǰ�� SysTick ������ֵ
    startTick = SysTick->VAL;
	
	do
    {
        // ��ȡ��ǰ�� SysTick ������ֵ
        currentTick = SysTick->VAL;

        // ������ĵδ���,��Ϊ�ǵݼ�������
        if (startTick > currentTick)
        {
			tickcnt+=startTick - currentTick;
        }
        else
        {
			// ������ SysTick �����������
			tickcnt+=startTick + (SysTick->LOAD - currentTick);
        }
		
		startTick=currentTick;
		
		if(tickcnt>=ticks)
			break; //ʱ�䳬������Ҫ�ӳٵ�ʱ��,���˳�.
		
    } while (1);

}

