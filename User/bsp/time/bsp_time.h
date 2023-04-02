#ifndef __BSP_TIME_H
#define __BSP_TIME_H
#include "stm32f10x.h"

/*********************************����PWM2������*********************************************/
#define FOC_SVPWM_PWM_CLOCK      RCC_APB1Periph_TIM2   //PWMʱ�����36M
#define FOC_SVPWM_GPIO_CLOCK     RCC_APB2Periph_GPIOA  //����ʱ��
#define FOC_SVPWM_GPIO_PIN       GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2  
#define FOC_SVPWM_GPIO_PORT      GPIOA
// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define FOC_SVPWM_TIM            TIM2
#define FOC_SVPWM_TIM_Prescaler  (10-1)        //��Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
#define FOC_SVPWM_TIM_PSC        (144-1)       //�����¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ

/******************************************************************************/


void TIM2_PWM_Init(void);//TIM2��ʼ��
/******************************************************************************/

#endif  /* _USART_H */
