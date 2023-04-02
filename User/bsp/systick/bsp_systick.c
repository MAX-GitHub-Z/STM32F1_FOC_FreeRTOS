#include "bsp_systick.h"

void SysTick_Delsy_us(uint32_t us)
{
	uint32_t i;
	
	SysTick_Config(72);//����ϵͳ��ʱ��
	
	for(i=0;i<us;i++)
	{
		while(!((SysTick->CTRL)&(1<<16)));//�ȴ�ʱ�䵽��
	}
		
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}
void SysTick_Delsy_ms(uint32_t ms)
{
	uint32_t i;
	
	SysTick_Config(72000);//����ϵͳ��ʱ��
	
	for(i=0;i<ms;i++)
	{
		while(!((SysTick->CTRL)&(1<<16)));//�ȴ�ʱ�䵽��
	}
		
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}
