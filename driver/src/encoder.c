/*
================================================================================
* File Name                 :   encoder.c
* Copyright                 :   2011-2013
* 
* CPU                       :   STM32F103
* RTOS                      :   FreeRTOS
*
* Create Date               :   2012/12/1
* Author/Company            :   Zhan Yubo/UESTC
* Abstract Description      :   Configuration and Applications of Encoder.
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*	NO.1		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/

/* includes */
#include "includes.h"
#include "encoder.h"
#include "foc.h"
#include "dma.h"
#define PI (3.1416)
/* 全局变量 */

/*编码器的结构体*/
encoderReadType Encoder;

/*
* ENCODER INTERFACE:
* 编码器接口：
* EA->PA6->TIM3-CH1
* EB->PA7->TIM3-CH2
* EZ->PB2
*/


void Encoder_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*设置时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO + RCC_APB2Periph_GPIOA+ RCC_APB2Periph_GPIOB,ENABLE);
	
	/*设置IO口*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 + GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	/*设置为编码器模式*/
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge,TIM_ICPolarity_BothEdge);
	TIM_Cmd(TIM3,ENABLE);

	/*初始化计数器的值为0xffff的一半*/
	//ENCODER = 0x7fff;

}

/*
* 编码器计数任务
*/

void encoderTask ( void *pvParameters )
{
	portTickType xLastWakeTime;
	int last_V;
	const portTickType xTime = 1;//
	int revolution=2000;
	int pos;
	Encoder_Configuration();
    // 使用当前时间 初始化xLastWakeTime 变量
    xLastWakeTime = xTaskGetTickCount();
	vTaskDelete(NULL);
    for( ;; )
    {
         // 等待下一个循环
        vTaskDelayUntil( &xLastWakeTime, xTime );
         // 执行区域
        /*读出码盘的值*/
//				last_V = Encoder.V;
//        Encoder.V = (ENCODER - 0x7fff);
//				Encoder.A = Encoder.V-last_V;
//        ENCODER = 0x7fff;     
//        /*速度积分*/
//        Encoder.S += Encoder.V;
//				pos=Encoder.S%(revolution*4);
//				if(pos<0)
//				{
//					pos+=revolution*4;
//				}
//				Encoder.P = pos/2;
//				Iq=-Ia*SinTable[Encoder.P]+(0.57735*Ia+1.1547*Ib)*CosTable[Encoder.P];
//				Id=Ia*CosTable[Encoder.P]+(0.57735*Ia+1.1547*Ib)*SinTable[Encoder.P];				
    }

}

/*========================== END OF FILE =======================================*/
