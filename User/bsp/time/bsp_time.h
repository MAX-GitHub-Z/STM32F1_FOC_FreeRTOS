#ifndef __BSP_TIME_H
#define __BSP_TIME_H
#include "stm32f10x.h"

/*********************************配置PWM2电机输出*********************************************/
#define FOC_SVPWM_PWM_CLOCK      RCC_APB1Periph_TIM2   //PWM时钟最大36M
#define FOC_SVPWM_GPIO_CLOCK     RCC_APB2Periph_GPIOA  //引脚时钟
#define FOC_SVPWM_GPIO_PIN       GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2  
#define FOC_SVPWM_GPIO_PORT      GPIOA
// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define FOC_SVPWM_TIM            TIM2
#define FOC_SVPWM_TIM_Prescaler  (10-1)        //作为 TIMx 时钟频率除数的预分频值
#define FOC_SVPWM_TIM_PSC        (144-1)       //更新事件装入活动的自动重装载寄存器周期的值

/******************************************************************************/


void TIM2_PWM_Init(void);//TIM2初始化
/******************************************************************************/

#endif  /* _USART_H */
