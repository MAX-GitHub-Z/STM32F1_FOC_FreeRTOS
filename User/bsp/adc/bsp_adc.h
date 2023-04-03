#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f10x.h"

/************************ADC1*******************************/
//adc1时钟
#define ADC1_CLCK             RCC_APB2Periph_ADC1  

//ADC1引脚时钟
#define ADC1_GPIO_CLCK        RCC_APB2Periph_GPIOA
//ADC1引脚
#define ADC1_GPIO_PIN         GPIO_Pin_1
#define ADC1_GPIO_PORT        GPIOA


/************************ADC2*******************************/
//adc2时钟
#define ADC2_CLCK             RCC_APB2Periph_ADC2 

//ADC2引脚时钟
#define ADC2_GPIO_CLCK        RCC_APB2Periph_GPIOA
//ADC2引脚
#define ADC2_GPIO_PIN         GPIO_Pin_2
#define ADC2_GPIO_PORT        GPIOA




void ADC_Config(void);
#endif /* __BSP_ADC_H */
