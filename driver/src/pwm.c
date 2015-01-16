/*
================================================================================
* File Name                 :   pwm.c
* Copyright                 :   2011-2013
* 
* CPU                       :   STM32F103
* RTOS                      :   FreeRTOS
*
* Create Date               :   2012/12/1
* Author/Company            :   Zhan Yubo/UESTC
* Abstract Description      :   Configuration and Applications of PWM.
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*	NO.1		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/

#include "includes.h"
#include "pwm.h"
#include "hall.h"
#include "current.h"
#include "canopen.h"
/*
* PWM接口:
* PWM1->PA9->TIM1 CH2
* PWM2->PA10->TIM1 CH3
* PWM3->PA8->TIM1 CH1
* ENA->PB14
* ENB->PB15
* ENC->PB13
*/

void pwmConfiguration( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA+RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);		
	
	/*NVIC*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 + GPIO_Pin_9 + GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//en pa6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13+GPIO_Pin_14+GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);


	TIM_CounterModeConfig(TIM1,TIM_CounterMode_CenterAligned3);
	/*Set pwm mode*/
	TIM_OCInitStructure.TIM_OCMode =  TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);

	TIM_SetCompare1(TIM1,0);//
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,0);//
	TIM_SetCompare4(TIM1, T_PWM-30);//
	
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_OC4Ref);
	
	TIM_SetAutoreload(TIM1,T_PWM-1);//f = 50k
//	
//	TIM_SetCompare1(TIM1,T_PWM/2);//
//	TIM_SetCompare2(TIM1,T_PWM/3);//
//	TIM_SetCompare3(TIM1,T_PWM/4);//
//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);
	
	TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);
	TIM_CCxCmd(TIM1,TIM_Channel_2,TIM_CCx_Enable);
	TIM_CCxCmd(TIM1,TIM_Channel_3,TIM_CCx_Enable);
	TIM_CCxCmd(TIM1,TIM_Channel_4,TIM_CCx_Enable);

	TIM_CtrlPWMOutputs( TIM1, ENABLE );
	TIM_Cmd(TIM1,ENABLE);
	TIM_ITConfig(TIM1,TIM_IT_CC4,ENABLE); //中断使能

}

void TIM1_CC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_CC4) == SET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_CC4);
		//hallValue = (GPIOHALL->IDR&(GPIO_Pin_HALL1+GPIO_Pin_HALL2+GPIO_Pin_HALL3));
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);

					
		//ADC_SoftwareStartConvCmd(ADC2,ENABLE);	

	}
}

/*========================== END OF FILE =======================================*/
