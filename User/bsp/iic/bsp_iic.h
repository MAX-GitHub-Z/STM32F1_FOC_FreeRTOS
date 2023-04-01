#ifndef __IIC_H
#define	__IIC_H

#include "stm32f10x.h"

//-----------------AS5600 IIC端口定义-------------------- 
//-----------------	B6 -> SCL-------------------------
//-----------------	B7 -> SDA------------------------- 					   

#define IIC_GPIO_SCL_CLK 	     RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define IIC_GPIO_SCL_PIN       GPIO_Pin_6
#define IIC_GPIO_SCL_PORT      GPIOB


#define IIC_GPIO_SDA_CLK 	     RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define IIC_GPIO_SDA_PIN       GPIO_Pin_7
#define IIC_GPIO_SDA_PORT      GPIOB

#define IIC_SCLK_Clr()  GPIO_ResetBits(IIC_GPIO_SCL_PORT,IIC_GPIO_SCL_PIN)    //
#define IIC_SCLK_Set()  GPIO_SetBits(IIC_GPIO_SCL_PORT,IIC_GPIO_SCL_PIN)      //

#define IIC_SDIN_Clr()  GPIO_ResetBits(IIC_GPIO_SDA_PORT,IIC_GPIO_SDA_PIN)
#define IIC_SDIN_Set()  GPIO_SetBits(IIC_GPIO_SDA_PORT,IIC_GPIO_SDA_PIN)
#define IIC_SDIN_READ() GPIO_ReadInputDataBit(IIC_GPIO_SDA_PORT,IIC_GPIO_SDA_PIN)

#define AS5600_RAW_ADDR    0X36
#define AS5600_WRITE_ADDR  (AS5600_RAW_ADDR << 1)
#define AS5600_READ_ADDR   ((AS5600_RAW_ADDR << 1)|1)

#define Read_AS5600_H 		 0x0c
#define Read_AS5600_L 		 0X0d


void IIC_GPIO_Config(void);
uint16_t AS5600_ReadTwoByte(void);

#endif /* __IIC_H */


