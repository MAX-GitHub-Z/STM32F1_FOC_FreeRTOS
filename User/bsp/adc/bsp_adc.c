#include "bsp_adc.h"

static void ADC1_Init()
{
	ADC_InitTypeDef ADC1_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(ADC1_GPIO_CLCK|ADC1_CLCK,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	//
	GPIO_InitStructure.GPIO_Pin=ADC1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(ADC1_GPIO_PORT,&GPIO_InitStructure);
	
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	
	ADC1_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC工作模式:ADC1同步规则组模式
	ADC1_InitStructure.ADC_ScanConvMode =DISABLE;     //模数转换工作在非扫描模式
	ADC1_InitStructure.ADC_ContinuousConvMode = DISABLE;    //模数转换工作在单次转换模式
	ADC1_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC1_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	  //ADC数据右对齐
	ADC1_InitStructure.ADC_NbrOfChannel = 1;	   //顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC1_InitStructure);	  //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器 

  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );
	
	// 开启ADC的DMA支持
	//ADC_DMACmd(ADC1, ENABLE);  //使能ADC的DMA位
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE); //使能ADC1

	/* Enable ADC1 reset calibaration register  使能ADC1复位校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register   ADC1复位校准寄存器检查结束*/
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibaration    启动ADC1校准 */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration  ADC1校准检查结束 */
	while(ADC_GetCalibrationStatus(ADC1));

}

static void ADC2_Init()
{
  ADC_InitTypeDef ADC2_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(ADC2_CLCK|ADC2_GPIO_CLCK,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	//
	GPIO_InitStructure.GPIO_Pin=ADC2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(ADC2_GPIO_PORT,&GPIO_InitStructure);
	
	ADC_DeInit(ADC2);  //复位ADC2,将外设 ADC2 的全部寄存器重设为缺省值
	
	ADC2_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC工作模式:ADC1同步规则组模式
	ADC2_InitStructure.ADC_ScanConvMode =DISABLE; //模数转换工作在非扫描模式
	ADC2_InitStructure.ADC_ContinuousConvMode = DISABLE; //模数转换工作在单次转换模式
	ADC2_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	  //转换由软件而不是外部触发启动
	ADC2_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC2_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC2, &ADC2_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

	ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5 );
	
	ADC_ExternalTrigConvCmd(ADC2, ENABLE);   //使能ADC2的外部触发模式 
 
    /* Enable ADC2 */
	ADC_Cmd(ADC2, ENABLE); //使能ADC2

	/* Enable ADC1 reset calibaration register  使能ADC2复位校准寄存器 */   
	ADC_ResetCalibration(ADC2);
	/* Check the end of ADC1 reset calibration register   ADC2复位校准寄存器检查结束*/
	while(ADC_GetResetCalibrationStatus(ADC2));

	/* Start ADC1 calibaration    启动ADC2校准 */
	ADC_StartCalibration(ADC2);
	/* Check the end of ADC1 calibration  ADC2校准检查结束 */
	while(ADC_GetCalibrationStatus(ADC2));

	
}

void ADC_Config()
{
	ADC1_Init();
	ADC2_Init();
}

