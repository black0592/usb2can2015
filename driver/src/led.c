/*
================================================================================
* File Name                 :    led.c
* Copyright                 :    2011-2013
* CPU                       :		 STM32F103
* RTOS                      :		 FreeRTOS
*
* Create Date               :    2011/9/30
* Author/Company            :		 Zhan Yubo
* Abstract Description      :		 Configuration and Applications of Led.
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
* NO.1 		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/

#include "led.h"
#include "includes.h"

int ledColour = TEST;//RED GREEN TEST
int ledState = IDLE;// ON OFF IDLE
int ledDelay = 100;
/*
* Simple Flashing
*/

void FLASHING_Once(void)
{
   LedON();
   delay_ms(100);
   LedOFF();
   delay_ms(100);
}

/*
* A simple delay.
*/
void delay_ms(unsigned int x)   //READ
{
  int i,j;
  for(i=0;i<x;i++)
  for(j=0;j<13150;j++);
}

/*
* IO Configuration
*/                                                                                    
void LED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PB4 is used for SWJ so we have to release it from JNTRST
  //AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_NOJNTRST;//Full SWJ (JTAG-DP + SW-DP) but without JNTRST 
	/*PB-LED*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED_PIN0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_PORT0,&GPIO_InitStructure);
}

void LedOn( int ledColour )
{
	if(ledColour == GREEN)
	{
		GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_RESET);
		GPIO_WriteBit(LED_PORT0,LED_PIN1,Bit_SET);
		GPIO_WriteBit(LED_PORT0,LED_PIN2,Bit_RESET);	
	}
	else if(ledColour == RED)
	{
		GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_RESET);
		GPIO_WriteBit(LED_PORT0,LED_PIN1,Bit_RESET);
		GPIO_WriteBit(LED_PORT0,LED_PIN2,Bit_SET);		
	}
	else if(ledColour == TEST)
	{
		GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_SET);	
		GPIO_WriteBit(LED_PORT0,LED_PIN1,Bit_RESET);
		GPIO_WriteBit(LED_PORT0,LED_PIN2,Bit_RESET);
	}
}
void LedOff(void)
{
	if((ledColour == RED)||(ledColour == GREEN))
	{
		GPIO_WriteBit(LED_PORT0,LED_PIN1,Bit_RESET);
		GPIO_WriteBit(LED_PORT0,LED_PIN2,Bit_RESET);	
	}
	else if(ledColour == TEST)
	{
		GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_RESET);		
	}
}


/*
*	Led Flashing Task
*/
void ledTask( void *pvParameters )
{
	/* Configure the IO of LED */
	LED_Configuration();

	for( ;; )
	{
			switch( ledState )
			{
				case IDLE:
					LedOn(ledColour);
					vTaskDelay(ledDelay);
					LedOff();
					vTaskDelay(ledDelay);	
					break;
				case FLASHING: 
					LedOn(ledColour);
					vTaskDelay(ledDelay);
					LedOff();
					vTaskDelay(ledDelay);				
					break;
				case ON: 
					LedOn(ledColour);
					vTaskDelay(ledDelay);
					break;
				case OFF:
					LedOff();
					vTaskDelay(ledDelay);
					break;
				default :break;
			}				
	}
}

void LEDALLOFF(void)
{
		GPIO_WriteBit(LED_PORT0,LED_PIN1,Bit_RESET);
		GPIO_WriteBit(LED_PORT0,LED_PIN2,Bit_RESET);
		GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_RESET);		
}
/*========================== END OF FILE =======================================*/

