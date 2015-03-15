/*
================================================================================
* File Name                 :	   MAIN.C
* Copyright                 :	   2012-2013
* 
* CPU                       :	   STM32F103
* RTOS                      :	   FreeRTOS
*
* Create Date               :	   2012/2/6
* Author/Company            :	   Zhan Yubo 
* Abstract Description      :    This program is for Motor control.
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
* NO.1		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/

/* INCLUDE */
#include "includes.h"
#include "led.h"
#include "main.h"
#include "can.h"
#include "sci.h"
#include "cli.h"

static void prvSetupHardware( void );
void startTask ( void *pvParameters );


xTaskHandle ledTaskHandle;
xTaskHandle sciTaskHandle;
xTaskHandle CLITaskHandle;

int main(void)
{
	prvSetupHardware();
	xTaskCreate( startTask, "System_start", configMINIMAL_STACK_SIZE*2, NULL, 10, NULL );
	vTaskStartScheduler();	
	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}

static void prvSetupHardware( void )
{
	//serialPortInit();
	CAN_Configuration();
//	initAllParameters(&memory);
}

void startTask ( void *pvParameters )
{

	xTaskCreate( ledTask, "led", configMINIMAL_STACK_SIZE, NULL, 1, &ledTaskHandle );
	xTaskCreate( canTestTask, "CAN test", configMINIMAL_STACK_SIZE*3, NULL, 2, NULL );
	xTaskCreate( sciSendTask, "sciSend", configMINIMAL_STACK_SIZE, NULL, 3, &sciTaskHandle );
	xTaskCreate( CLITask, "CLI", configMINIMAL_STACK_SIZE, NULL, 5, &CLITaskHandle );
	
	for( ;; )
	{				
		vTaskDelete(NULL);
	}
}

/*========================== END OF FILE =======================================*/
