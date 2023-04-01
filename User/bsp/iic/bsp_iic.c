#include "bsp_iic.h"



void IIC_GPIO_Config()
{
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;
	
	//开启iic的IO时钟
	RCC_APB2PeriphClockCmd(IIC_GPIO_SCL_CLK|IIC_GPIO_SDA_CLK,ENABLE);
	
	
	
	/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = IIC_GPIO_SCL_PIN;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(IIC_GPIO_SCL_PORT, &GPIO_InitStructure);	
	
	
	
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = IIC_GPIO_SDA_PIN;	

		/*设置引脚模式为通用开路输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(IIC_GPIO_SDA_PORT, &GPIO_InitStructure);

}

static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：MCU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}
//起始信号
 void i2c_Start()		
{
	IIC_SDIN_Set();
	IIC_SCLK_Set();
	i2c_Delay();
	IIC_SDIN_Clr();
	i2c_Delay();
	IIC_SCLK_Clr();
	i2c_Delay();
}
	//终止信号
 void i2c_Stop()			
{	
	IIC_SCLK_Clr() ;
	IIC_SDIN_Clr();
	i2c_Delay();
	IIC_SCLK_Set();

}


/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: MCU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			IIC_SDIN_Set();
		}
		else
		{
			IIC_SDIN_Clr();
		}
		i2c_Delay();
		IIC_SCLK_Set();
		i2c_Delay();	
		IIC_SCLK_Clr();
		if (i == 7)
		{
			 IIC_SDIN_Set(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}
/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: MCU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		IIC_SCLK_Set();
		i2c_Delay();
		if (IIC_SDIN_READ())
		{
			value++;
		}
		IIC_SCLK_Clr();
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: MCU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	IIC_SDIN_Set();	/* MCU释放SDA总线 */
	i2c_Delay();
	IIC_SCLK_Set();	/* MCU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (IIC_SDIN_READ())	/* MCU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	IIC_SCLK_Clr();
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: MCU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	IIC_SDIN_Clr();	/* MCU驱动SDA = 0 */
	i2c_Delay();
	IIC_SCLK_Set();	/* MCU产生1个时钟 */
	i2c_Delay();
	IIC_SCLK_Clr();
	i2c_Delay();
	IIC_SDIN_Set();	/* MCU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: MCU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	IIC_SDIN_Set();	/* MCU驱动SDA = 1 */
	i2c_Delay();
	IIC_SCLK_Set();	/* MCU产生1个时钟 */
	i2c_Delay();
	IIC_SCLK_Clr();
	i2c_Delay();	
}

/*
写并读出一个数据

*/
uint8_t AS5600_ReadOneByte(uint16_t ReadAddr)
{
	uint8_t temp=-1;
	i2c_Start();
	i2c_SendByte(AS5600_WRITE_ADDR);//写地址
	i2c_WaitAck();
	i2c_SendByte(ReadAddr);//写寄存器
	i2c_WaitAck();
	
	
	i2c_Start();
	i2c_SendByte(AS5600_READ_ADDR);//读地址
	i2c_WaitAck();
	temp=i2c_ReadByte();
	i2c_Stop();
	return temp;
}

/*
读出两个数据

*/
uint16_t AS5600_ReadTwoByte()
{
	uint16_t twoByte_Data=-1;
	uint8_t H_Data=0,L_Data=1;
	H_Data=AS5600_ReadOneByte(Read_AS5600_H);
	L_Data=AS5600_ReadOneByte(Read_AS5600_L);
	twoByte_Data=(H_Data<<8)|L_Data;
	return twoByte_Data;
}









