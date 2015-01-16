/*
================================================================================
* File Name                 :   ad.c
* Copyright                 :   2011-2013
* 
* CPU                       :   STM32F103
* RTOS                      :   FreeRTOS
*
* Create Date               :   2012/12/1
* Author/Company            :   Zhan Yubo/UESTC
* Abstract Description      :   Configuration and Applications of ADC.
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*	NO.1		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/

#include "includes.h"
//#include "hall.h"
//#include "current.h"
#include "ad.h"
//#include "pwm.h"
//#include "memory.h"
#//include "dma.h"

/*
* ADC1->PA4 ->ADCIN4 ->B相电流采样
* ADC2->PA3 ->ADCIN3 ->A相电流采样
* ADC3->PA5 ->ADCIN5 ->外部模拟量输入
* ADC4->PB0 ->ADCIN8 ->电源电压采样
*/

/****
	* ad初始化
	*/
int adc_bus;
int adc_extern;
int adc_d5v;

void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/*RCC*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO+RCC_APB2Periph_GPIOA+RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);

	/*NVIC*/
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*GPIO-PA3-PA4-PA5*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3+GPIO_Pin_4+GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/*GPIO-PB0*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	/*Perigh*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 4;
	ADC_Init(ADC1,&ADC_InitStructure);
	
// 	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
// 	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
// 	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
// 	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
// 	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
// 	ADC_InitStructure.ADC_NbrOfChannel = 1;
// 	ADC_Init(ADC2,&ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,1,ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,2,ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,3,ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,4,ADC_SampleTime_1Cycles5);
	
// 	ADC_ExternalTrigConvCmd(ADC1,ENABLE);
// 	ADC_ExternalTrigConvCmd(ADC2,ENABLE);
	DMA_Configuration();
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	//ADC_DMACmd(ADC2,ENABLE);
	//ADC_Cmd(ADC2,ENABLE);
	ADC_ResetCalibration(ADC1);
	//ADC_ResetCalibration(ADC2);
	while(ADC_GetResetCalibrationStatus(ADC1));
	//while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC1);
	//ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC1));
	//while(ADC_GetCalibrationStatus(ADC2));
	//ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	
	//ADC_SoftwareStartInjectedConvCmd(ADC1,ENABLE);
	//ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	//ADC_SoftwareStartConvCmd(ADC2,ENABLE);
}

/****
	* AD中断函数
	*/
void ADC1_2_IRQHandler(void)
{
	if((ADC1->SR&0X02)==0X02)
	{
		ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);

	}
	if(ADC_GetITStatus(ADC2,ADC_IT_EOC) == SET)
	{
		ADC_ClearITPendingBit(ADC2,ADC_IT_EOC);
		
	}
	
	ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
	//ADC_ClearFlag(ADC2,ADC_FLAG_EOC);
}
/*========================== END OF FILE =======================================*/
