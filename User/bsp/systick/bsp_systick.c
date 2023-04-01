#include "bsp_systick.h"

void SysTick_Delsy_us(uint32_t us)
{
	uint32_t i;
	
	SysTick_Config(72);//定义系统定时器
	
	for(i=0;i<us;i++)
	{
		while(!((SysTick->CTRL)&(1<<16)));//等待时间到达
	}
		
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}
void SysTick_Delsy_ms(uint32_t ms)
{
	uint32_t i;
	
	SysTick_Config(72000);//定义系统定时器
	
	for(i=0;i<ms;i++)
	{
		while(!((SysTick->CTRL)&(1<<16)));//等待时间到达
	}
		
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}
