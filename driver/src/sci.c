/*
================================================================================
* File Name                 :   sci.c
* Copyright                 :   2011-2013
* 
* CPU                       :   STM32F103
* RTOS                      :   FreeRTOS
*
* Create Date               :   2012/12/1
* Author/Company            :   Zhan Yubo/UESTC
* Abstract Description      :   Configuration and Applications of Serial Communication Interface.
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*	NO.1		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/
#include "includes.h"
#include "sci.h"
#include "led.h"
#include <string.h>
#include "main.h"
#include "cli.h"

xQueueHandle bMessage;
cmdType cmdBuf;

/*
* Configuration of Serial Communication Interface
*/
void sciConfiguration( void )
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB+RCC_APB2Periph_AFIO,ENABLE); /* 开启对应的IO时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);/* 开启串口1时钟 */

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;    /* 选择IRQ通道 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  /* 设置成员NVIC_IRQChannel的先占优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         /* 设置成员NVIC_IRQChannel的从优先级 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            /* 使能中断功能 */
    NVIC_Init(&NVIC_InitStructure);
		NVIC_SetPriority(USART3_IRQn,15);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;      
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;      
    GPIO_Init(GPIOB , &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;             
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB , &GPIO_InitStructure);

    USART_DeInit(USART3); /* 复位USART3 */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART_Init(USART3,&USART_InitStructure);
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);/* 开启接收中断 */
		USART_Cmd(USART3,ENABLE);
		//USART3->DR = 0Xaa;
}

/****
	*	Send a char
	*/
void sciSendChar( unsigned char Data, xQueueHandle messageQueue )
{
	xQueueSend( messageQueue, &Data, 10 );
}

/****
	*	Send a halfword
	*/
void sciSendHalfword( short halfword, xQueueHandle messageQueue )
{
	unsigned char temp;

	temp = (unsigned char)((halfword&0xff00)>>8);
	sciSendChar( temp, bMessage );
	temp = (unsigned char)(halfword&0xff);
	sciSendChar( temp, bMessage );	
}

/****
	*	Send a word
	*/
void sciSendWord ( long word, xQueueHandle messageQueue )
{
	unsigned char temp;

	temp = (unsigned char)((word&0xff000000)>>24);
	sciSendChar( temp, bMessage );
	temp = (unsigned char)((word&0xff0000)>>16);
	sciSendChar( temp, bMessage );
	temp = (unsigned char)((word&0xff00)>>8);
	sciSendChar( temp, bMessage );
	temp = (unsigned char)(word&0xff);
	sciSendChar( temp, bMessage );		
}

void sciSendString( char * addr, xQueueHandle messageQueue )
{
	while( *addr != '\0' )
	{
		sciSendChar( *addr, messageQueue );
		addr++;
	}
}

char* int2String( int number, char* stringBuf )
{
	int temp = 10;
	int n = 1;
	char* stringHead;

	stringHead = stringBuf;
	
	if( number<0 )
	{
		number = -number;
		*stringBuf = '-';
		stringBuf++;
	}
	
	while( (number/temp) != 0 )
	{
		temp = temp*10;
		n++;
	}
	while( n-- )
	{
		temp = temp/10;
		
		switch( (number/temp)%10 )
		{
			case 0: *stringBuf = '0'; break;
			case 1: *stringBuf = '1'; break;
			case 2: *stringBuf = '2'; break;
			case 3: *stringBuf = '3'; break;
			case 4: *stringBuf = '4'; break;
			case 5: *stringBuf = '5'; break;
			case 6: *stringBuf = '6'; break;
			case 7: *stringBuf = '7'; break;
			case 8: *stringBuf = '8'; break;
			case 9: *stringBuf = '9'; break;
			default: break;
		}
		stringBuf ++;
	}
	*stringBuf = '\0';

	return stringHead;
}

char* float2String( float number, unsigned char resolution, char* stringBuf )
{
	char *stringHead = stringBuf;
	char *stringEnd = stringBuf;
	long n = 0;
	long m =0;
	int digit = 0;
	int i=0;
	int temp=0;

	if(number <0)
	{
		number = -number;
		*stringEnd = '-';
		stringEnd++;		
	}
	if(number>65536.0)
	{
		return 0;
	}
	n=number*(int)pow(10,resolution);
	m=n;
	while(m)
	{
		m=m/10;
		digit++;
	}
	if(digit<=resolution)
		digit = resolution+1;
	for(i=digit;i>0;i--)
	{
		if(i==resolution)
		{
			*stringEnd = '.';
			stringEnd++;
		}
		temp = n%(int)(pow(10,i))/(int)(pow(10,i-1));			
		switch(temp)
		{
			case 0: *stringEnd = '0'; break;
			case 1: *stringEnd = '1'; break;
			case 2: *stringEnd = '2'; break;
			case 3: *stringEnd = '3'; break;
			case 4: *stringEnd = '4'; break;
			case 5: *stringEnd = '5'; break;
			case 6: *stringEnd = '6'; break;
			case 7: *stringEnd = '7'; break;
			case 8: *stringEnd = '8'; break;
			case 9: *stringEnd = '9'; break;
			default: *stringEnd = '-'; break;			
		}
		stringEnd++;
	}
	*stringEnd = '\0';
	return stringHead;
	
}

char* hex2String( long number, char* stringBuf )
{
	int temp = 16;
	int n = 1;
	char* stringHead;

	stringHead = stringBuf;
	
	if( number<0 )
	{
		number = -number;
		*stringBuf = '-';
		stringBuf++;
	}
	
	while( (number/temp) != 0 )
	{
		temp = temp*16;
		n++;
	}
	while( n-- )
	{
		temp = temp/16;
		
		switch( (number/temp)%16 )
		{
			case 0: *stringBuf = '0'; break;
			case 1: *stringBuf = '1'; break;
			case 2: *stringBuf = '2'; break;
			case 3: *stringBuf = '3'; break;
			case 4: *stringBuf = '4'; break;
			case 5: *stringBuf = '5'; break;
			case 6: *stringBuf = '6'; break;
			case 7: *stringBuf = '7'; break;
			case 8: *stringBuf = '8'; break;
			case 9: *stringBuf = '9'; break;
			case 10: *stringBuf = 'A'; break;
			case 11: *stringBuf = 'B'; break;
			case 12: *stringBuf = 'C'; break;
			case 13: *stringBuf = 'D'; break;
			case 14: *stringBuf = 'E'; break;
			case 15: *stringBuf = 'F'; break;

			default: break;
		}
		stringBuf ++;
	}
	*stringBuf = '\0';

	return stringHead;	
}

//
void USART3_IRQHandler(void)
{
	char inData;
	
	USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	inData = USART3->DR;
	if(inData == '\r')
	{
		while(cmdBuf.head<CMDBUFSIZE)
		{
			cmdBuf.String[cmdBuf.head] = '\0';
			cmdBuf.head++;
		}
		cmdBuf.head = 0;
		cmdBuf.FinishFlag = 1;
	}
	else
	{
		if(inData!='\n')
		{
			cmdBuf.String[cmdBuf.head] = inData;
			cmdBuf.head++;
		}	
		if(cmdBuf.head>=CMDBUFSIZE)
		{
			cmdBuf.BufOverErrorFlag = 1;
			cmdBuf.head = 0;
		}
	}
	
}

void sciSendTask ( void *pvParameters )
{
	unsigned char buf;
	cmdBuf.head =0;
	sciConfiguration();	
	bMessage = xQueueCreate( 50, sizeof(unsigned char) );

	if( bMessage != 0 )
	{
		for( ;; )
		{
			if( xQueueReceive( bMessage, &buf, 10 ) == pdTRUE )
			{
				while (!(USART3->SR & 0x0080));
				USART3->DR = buf;	
			}
		}
	}
	else
	{
		USART3->DR = 0X55;//队列创建不成功
		vTaskDelete( NULL );
	}
}
/*
* Task of Command Line Interface
*/
void CLITask ( void *pvParameters )
{
	vTaskDelay(500);
	sciSendString("\r\nWelcom to Robot Learning System!",bMessage);
	for(;;)
	{
		vTaskDelay(20);
		if(cmdBuf.BufOverErrorFlag == 1)
		{
			cmdBuf.BufOverErrorFlag = 0;
			sciSendString("\r\nCommand is too long!",bMessage);
		}
		/* cmd analysis */
		if( cmdBuf.FinishFlag == 1 )
		{
			cmdBuf.FinishFlag =0;
			findCommandInList(cmdBuf.String);
		}
	}
}
/*========================== END OF FILE =======================================*/
