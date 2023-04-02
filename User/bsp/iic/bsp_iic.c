#include "bsp_iic.h"



void IIC_GPIO_Config()
{
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;
	
	//����iic��IOʱ��
	RCC_APB2PeriphClockCmd(IIC_GPIO_SCL_CLK|IIC_GPIO_SDA_CLK,ENABLE);
	
	
	
	/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = IIC_GPIO_SCL_PIN;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(IIC_GPIO_SCL_PORT, &GPIO_InitStructure);	
	
	
	
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = IIC_GPIO_SDA_PIN;	

		/*��������ģʽΪͨ�ÿ�·���*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(IIC_GPIO_SDA_PORT, &GPIO_InitStructure);

}

static void i2c_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������MCU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 10; i++);
}
//��ʼ�ź�
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
	//��ֹ�ź�
 void i2c_Stop()			
{	
	IIC_SCLK_Clr() ;
	IIC_SDIN_Clr();
	i2c_Delay();
	IIC_SCLK_Set();

}


/*
*********************************************************************************************************
*	�� �� ��: i2c_SendByte
*	����˵��: MCU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
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
			 IIC_SDIN_Set(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}
/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: MCU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
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
*	�� �� ��: i2c_WaitAck
*	����˵��: MCU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	IIC_SDIN_Set();	/* MCU�ͷ�SDA���� */
	i2c_Delay();
	IIC_SCLK_Set();	/* MCU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (IIC_SDIN_READ())	/* MCU��ȡSDA����״̬ */
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
*	�� �� ��: i2c_Ack
*	����˵��: MCU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	IIC_SDIN_Clr();	/* MCU����SDA = 0 */
	i2c_Delay();
	IIC_SCLK_Set();	/* MCU����1��ʱ�� */
	i2c_Delay();
	IIC_SCLK_Clr();
	i2c_Delay();
	IIC_SDIN_Set();	/* MCU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: MCU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	IIC_SDIN_Set();	/* MCU����SDA = 1 */
	i2c_Delay();
	IIC_SCLK_Set();	/* MCU����1��ʱ�� */
	i2c_Delay();
	IIC_SCLK_Clr();
	i2c_Delay();	
}

/*
д������һ������

*/
uint8_t AS5600_ReadOneByte(uint16_t ReadAddr)
{
	uint8_t temp=-1;
	i2c_Start();
	i2c_SendByte(AS5600_WRITE_ADDR);//д��ַ
	i2c_WaitAck();
	i2c_SendByte(ReadAddr);//д�Ĵ���
	i2c_WaitAck();
	
	
	i2c_Start();
	i2c_SendByte(AS5600_READ_ADDR);//����ַ
	i2c_WaitAck();
	temp=i2c_ReadByte();
	i2c_Stop();
	return temp;
}

/*
������������

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









