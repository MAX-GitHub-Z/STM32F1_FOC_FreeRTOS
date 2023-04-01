#include "bsp_time.h"

void TIM2_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;               // GPIO Configuration  结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//  TIM Configuration
    TIM_OCInitTypeDef  TIM_OCInitStructure;           // Oscillator Configuration

    RCC_APB1PeriphClockCmd(FOC_SVPWM_PWM_CLOCK,ENABLE); // Enable定时器时钟
    RCC_APB2PeriphClockCmd(FOC_SVPWM_GPIO_CLOCK,ENABLE); // Enable引脚时钟

    GPIO_InitStructure.GPIO_Pin =FOC_SVPWM_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_Init(FOC_SVPWM_GPIO_PORT,&GPIO_InitStructure);

    TIM_DeInit(FOC_SVPWM_TIM);
    TIM_TimeBaseInitStructure.TIM_Prescaler=FOC_SVPWM_TIM_Prescaler;
    TIM_TimeBaseInitStructure.TIM_Period=FOC_SVPWM_TIM_PSC;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//不进行时钟分割
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned1;//中央对齐
    TIM_TimeBaseInit(FOC_SVPWM_TIM,&TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//tim1脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//输出比较使能
    TIM_OCInitStructure.TIM_Pulse=0;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出比较极性高
    TIM_OC1Init(FOC_SVPWM_TIM,&TIM_OCInitStructure);//初始化通道1
    TIM_OC2Init(FOC_SVPWM_TIM,&TIM_OCInitStructure);//初始化通道2
    TIM_OC3Init(FOC_SVPWM_TIM,&TIM_OCInitStructure);//初始化通道3
    TIM_OC1PreloadConfig(FOC_SVPWM_TIM,TIM_OCPreload_Enable);//使能CCR1
    TIM_OC2PreloadConfig(FOC_SVPWM_TIM,TIM_OCPreload_Enable);//使能CCR2
    TIM_OC3PreloadConfig(FOC_SVPWM_TIM,TIM_OCPreload_Enable);//使能CCR3

    TIM_ARRPreloadConfig(FOC_SVPWM_TIM,ENABLE);
    TIM_Cmd(FOC_SVPWM_TIM,ENABLE);
}


