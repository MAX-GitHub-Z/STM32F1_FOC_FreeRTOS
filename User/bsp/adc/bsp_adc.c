#include "bsp_adc.h"

static void ADC1_Init()
{
	ADC_InitTypeDef ADC1_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(ADC1_GPIO_CLCK|ADC1_CLCK,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	//
	GPIO_InitStructure.GPIO_Pin=ADC1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(ADC1_GPIO_PORT,&GPIO_InitStructure);
	
	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	
	ADC1_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC����ģʽ:ADC1ͬ��������ģʽ
	ADC1_InitStructure.ADC_ScanConvMode =DISABLE;     //ģ��ת�������ڷ�ɨ��ģʽ
	ADC1_InitStructure.ADC_ContinuousConvMode = DISABLE;    //ģ��ת�������ڵ���ת��ģʽ
	ADC1_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC1_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	  //ADC�����Ҷ���
	ADC1_InitStructure.ADC_NbrOfChannel = 1;	   //˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC1_InitStructure);	  //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ��� 

  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );
	
	// ����ADC��DMA֧��
	//ADC_DMACmd(ADC1, ENABLE);  //ʹ��ADC��DMAλ
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1

	/* Enable ADC1 reset calibaration register  ʹ��ADC1��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register   ADC1��λУ׼�Ĵ���������*/
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibaration    ����ADC1У׼ */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration  ADC1У׼������ */
	while(ADC_GetCalibrationStatus(ADC1));

}

static void ADC2_Init()
{
  ADC_InitTypeDef ADC2_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(ADC2_CLCK|ADC2_GPIO_CLCK,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	//
	GPIO_InitStructure.GPIO_Pin=ADC2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(ADC2_GPIO_PORT,&GPIO_InitStructure);
	
	ADC_DeInit(ADC2);  //��λADC2,������ ADC2 ��ȫ���Ĵ�������Ϊȱʡֵ
	
	ADC2_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC����ģʽ:ADC1ͬ��������ģʽ
	ADC2_InitStructure.ADC_ScanConvMode =DISABLE; //ģ��ת�������ڷ�ɨ��ģʽ
	ADC2_InitStructure.ADC_ContinuousConvMode = DISABLE; //ģ��ת�������ڵ���ת��ģʽ
	ADC2_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	  //ת��������������ⲿ��������
	ADC2_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC2_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC2, &ADC2_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

	ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5 );
	
	ADC_ExternalTrigConvCmd(ADC2, ENABLE);   //ʹ��ADC2���ⲿ����ģʽ 
 
    /* Enable ADC2 */
	ADC_Cmd(ADC2, ENABLE); //ʹ��ADC2

	/* Enable ADC1 reset calibaration register  ʹ��ADC2��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC2);
	/* Check the end of ADC1 reset calibration register   ADC2��λУ׼�Ĵ���������*/
	while(ADC_GetResetCalibrationStatus(ADC2));

	/* Start ADC1 calibaration    ����ADC2У׼ */
	ADC_StartCalibration(ADC2);
	/* Check the end of ADC1 calibration  ADC2У׼������ */
	while(ADC_GetCalibrationStatus(ADC2));

	
}

void ADC_Config()
{
	ADC1_Init();
	ADC2_Init();
}

