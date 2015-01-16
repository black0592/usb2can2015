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
/* 函数声明 */
static void prvSetupHardware( void );
void startTask ( void *pvParameters );

/* 任务句柄 */
xTaskHandle ledTaskHandle;
xTaskHandle sciTaskHandle;
xTaskHandle CLITaskHandle;

/****
	*	主函数 
	*/
int main(void)
{
	prvSetupHardware();

	/*创建启动任务*/
	xTaskCreate( startTask, "System_start", configMINIMAL_STACK_SIZE*2, NULL, 10, NULL );

	/* 启动任务调度器 任务开始执行 */
	vTaskStartScheduler();	

	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	vTaskDelay(100);
	for( ;; );
	//return 0;
}

/****
	*	硬件初始化
	*/
static void prvSetupHardware( void )
{
	//serialPortInit();
	CAN_Configuration();
//	initAllParameters(&memory);
}

/****
	*	任务-启动任务 
	*/
void startTask ( void *pvParameters )
{

	/*创建任务-led*/
	xTaskCreate( ledTask, "led", configMINIMAL_STACK_SIZE, NULL, 1, &ledTaskHandle );
	
	/*创建任务-Encoder*/
	//xTaskCreate( encoderTask, "Encoder", configMINIMAL_STACK_SIZE, NULL, 1, &encoderTaskHandle );
	
	/*创建任务-CANtest*/
	xTaskCreate( canTestTask, "CAN test", configMINIMAL_STACK_SIZE*3, NULL, 2, NULL );
	
//	/*创建任务 - 串口*/
	xTaskCreate( sciSendTask, "sciSend", configMINIMAL_STACK_SIZE, NULL, 3, &sciTaskHandle );
//	
//	/*创建任务 - CLI */
	xTaskCreate( CLITask, "CLI", configMINIMAL_STACK_SIZE, NULL, 5, &CLITaskHandle );
	
	for( ;; )
	{				
		/* 删除自己 */			
		vTaskDelete(NULL);
	}
}

/*========================== END OF FILE =======================================*/
