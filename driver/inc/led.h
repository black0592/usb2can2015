/*
================================================================================
* File Name                 :    led.h
* Copyright                 :    2011-2013
* Module Name               :
* 
* CPU                       :    STM32F103VCT6(Cotex M3)
* RTOS                      :    None
*
* Create Date               :    2011/9/30
* Author/Company            :    战玉博/UESTC
* Abstract Description      :
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*
================================================================================
*/

/*******************************************************************************
*			Multi-Include-Prevent Section
*******************************************************************************/
#ifndef  _LED_H_
#define  _LED_H_


/*******************************************************************************
* Include File Section
*******************************************************************************/
#include "includes.h"

/*******************************************************************************
* Macro Define Section
*******************************************************************************/

/* led port */
#define LED_PORT0 GPIOB

/* led pin */
#define LED_PIN0 GPIO_Pin_12
#define LED_PIN1 GPIO_Pin_4
#define LED_PIN2 GPIO_Pin_5

/* led action */
#define LedON() GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_SET)
#define LedOFF() GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_RESET)
#define RED_LedON() GPIO_WriteBit(LED_PORT0,LED_PIN1,Bit_SET)
#define RED_LedOFF() GPIO_WriteBit(LED_PORT0,LED_PIN1,Bit_RESET)
#define BLUE_LedON() GPIO_WriteBit(LED_PORT0,LED_PIN1,Bit_SET)
#define BLUE_LedOFF() GPIO_WriteBit(LED_PORT0,LED_PIN1,Bit_RESET)

/* led colour */
#define RED 	0
#define GREEN 1
#define TEST 	2

/* led state */
#define FLASHING 	0
#define ON 				1
#define OFF 			2
#define IDLE 			3


/* 全局变量 */
extern int ledColour;
extern int ledState;
extern int ledDelay;
/*******************************************************************************
*	Prototype Declare Section
*******************************************************************************/
void Flash_Once(void);
void delay_ms(unsigned int x);
void ledTask( void *pvParameters );
void LEDALLOFF(void);
#endif

/*==========================END OF FILE=======================================*/

