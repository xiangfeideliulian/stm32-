#include "adc.h"
#include "delay.h"

//ADC初始化 - PA1 ADC1通道1
void My_ADC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	//使能GPIOA和ADC1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

	//设置ADC时钟分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72MHz/6=12MHz

	//配置PA1为模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//ADC基本配置
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	//使能ADC1
	ADC_Cmd(ADC1, ENABLE);

	//ADC校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
}

//读取ADC值 - 通道1 (PA1)
u16 ADC_Read(void)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);
}

//读取ADC并转换为实际电压值(0-3.3V)
//显示时，实际电压值会映射到0-5V坐标轴位置
float ADC_GetVoltage(void)
{
	u16 adc_value;
	float voltage;

	adc_value = ADC_Read();
	//计算实际电压 = adc_value * 3.3 / 4095 (范围0-3.3V)
	voltage = (float)adc_value * 3.3f / 4095.0f;

	return voltage;
}