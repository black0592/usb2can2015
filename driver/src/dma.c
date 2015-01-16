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
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;//�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC1_Value;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//dma���䷽����
	DMA_InitStructure.DMA_BufferSize = 4;//����DMA�ڴ���ʱ�������ĳ��� word
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//����DMA���ڴ����ģʽ��
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//����DMA�Ĵ���ģʽ���������ϵ�ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//����DMA�����ȼ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//����DMA��2��memory�еı����������
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
