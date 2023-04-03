#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f10x.h"

/************************ADC1*******************************/
//adc1ʱ��
#define ADC1_CLCK             RCC_APB2Periph_ADC1  

//ADC1����ʱ��
#define ADC1_GPIO_CLCK        RCC_APB2Periph_GPIOA
//ADC1����
#define ADC1_GPIO_PIN         GPIO_Pin_1
#define ADC1_GPIO_PORT        GPIOA


/************************ADC2*******************************/
//adc2ʱ��
#define ADC2_CLCK             RCC_APB2Periph_ADC2 

//ADC2����ʱ��
#define ADC2_GPIO_CLCK        RCC_APB2Periph_GPIOA
//ADC2����
#define ADC2_GPIO_PIN         GPIO_Pin_2
#define ADC2_GPIO_PORT        GPIOA




void ADC_Config(void);
#endif /* __BSP_ADC_H */
