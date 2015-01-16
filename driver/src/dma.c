/*
================================================================================
* File Name                 :   dma.c
* Copyright                 :   2011-2013
* 
* CPU                       :   STM32F103
* RTOS                      :   FreeRTOS
*
* Create Date               :   2012/12/1
* Author/Company            :   Zhan Yubo/UESTC
* Abstract Description      :   Configuration and Applications of DMA.
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*	NO.1		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/

#include "dma.h"
#include "includes.h"
#include "stm32_dsp.h"
#include "memory.h"
#include "canopen.h"

#include <math.h>
short ADC1_Value[5];
int adc1,adc2;
int adc;

	long Position[6];
	int Rev = 1000*2;
	long HallPos[6];
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
//#define ADC2_DR_Address (&(ADC2->DR))
void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*RCC*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE)	;
		
	/*NVIC*/
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;//外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC1_Value;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//dma传输方向单向
	DMA_InitStructure.DMA_BufferSize = 4;//设置DMA在传输时缓冲区的长度 word
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//设置DMA的内存递增模式，
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//内存数据字长
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//设置DMA的传输模式：连续不断的循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//设置DMA的优先级别
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//设置DMA的2个memory中的变量互相访问
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA1_Channel1,ENABLE);
}
void DMA1_Channel1_IRQHandler(void)
{
	
	if(DMA_GetITStatus(DMA1_IT_TC1) == SET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1);

		adc1 = (int)(2048-ADC1_Value[0]);
		adc2 = (int)(2048-ADC1_Value[1]);
//		Iq=-Ia*Sin(Encoder.P)+(0.57735*Ia+1.1547*Ib)*Cos(Encoder.P);
//		Id=Ia*Cos(Encoder.P)+(0.57735*Ia+1.1547*Ib)*Sin(Encoder.P);			
		hallValue = (GPIOHALL->IDR&(GPIO_Pin_HALL1+GPIO_Pin_HALL2+GPIO_Pin_HALL3));
		canopen_statemachine();
		



				
	}
}
/*========================== END OF FILE =======================================*/
