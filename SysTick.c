#include "public.h"




void SysTick_Init(void)
{
	//系统定时器频率为168Mhz  
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	
	/*重装载寄存器根据configTICK_RATE_HZ设定溢出时间，
	设计数值为x，频率为168*1000000HZ，中断周期为1/configTICK_RATE_HZ
	
	x * 1/(168*1000000)=1/ configTICK_RATE_HZ
	x= 168*1000000/ configTICK_RATE_HZ
	
	此为24位寄存器,最大值:16777216，在168M下,约合0.0998s左右，
	*/
	SysTick->LOAD =(168*1000000/configTICK_RATE_HZ)-1;
	// 清除当前计数值
    SysTick->VAL = 0; 
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启SYSTICK中断
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK 
	
}



/*
在 FreeRTOS 中使用 SysTick 作为延时函数需要小心，
因为 FreeRTOS 自己使用了 SysTick 作为系统时钟。
如果直接操作 SysTick 定时器，可能会干扰 FreeRTOS 的调度。
*/
void Delay_us(uint32_t  nTime)
{ 
	uint32_t ticks,tickcnt=0;
    uint32_t startTick, currentTick;
	
	// 将 nTime 转换为滴答数
    ticks = (168000000 / 1000000) * nTime;
	
	// 记录当前的 SysTick 计数器值
    startTick = SysTick->VAL;
	
	do
    {
        // 获取当前的 SysTick 计数器值
        currentTick = SysTick->VAL;

        // 计算过的滴答数,因为是递减计数器
        if (startTick > currentTick)
        {
			tickcnt+=startTick - currentTick;
        }
        else
        {
			// 发生了 SysTick 倒计数器溢出
			tickcnt+=startTick + (SysTick->LOAD - currentTick);
        }
		
		startTick=currentTick;
		
		if(tickcnt>=ticks)
			break; //时间超过等于要延迟的时间,则退出.
		
    } while (1);
	
}



void Delay_ms(uint32_t nTime)
{
	uint32_t ticks,tickcnt=0;
    uint32_t startTick, currentTick;
	
	// 将 nTime 转换为滴答数
    ticks = (168000000 / 1000) * nTime;
	
	// 记录当前的 SysTick 计数器值
    startTick = SysTick->VAL;
	
	do
    {
        // 获取当前的 SysTick 计数器值
        currentTick = SysTick->VAL;

        // 计算过的滴答数,因为是递减计数器
        if (startTick > currentTick)
        {
			tickcnt+=startTick - currentTick;
        }
        else
        {
			// 发生了 SysTick 倒计数器溢出
			tickcnt+=startTick + (SysTick->LOAD - currentTick);
        }
		
		startTick=currentTick;
		
		if(tickcnt>=ticks)
			break; //时间超过等于要延迟的时间,则退出.
		
    } while (1);

}

