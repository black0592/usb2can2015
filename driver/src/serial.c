#include "includes.h"
#include "serial.h"
#include "can.h"


serialObj serial;

void serialObjInit( void )
{
	serial.start = 0;
	serial.lastInData = 0;
	serial.n = 0;
	serial.sum = 0;
	serial.ID = 0;
	serial.Index = 0;
	serial.SubIndex = 0;
	serial.Data = 0;
}

void serialPortInit( void )
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
    USART_InitStructure.USART_BaudRate = 230400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART_Init(USART3,&USART_InitStructure);
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);/* 开启接收中断 */
		USART_Cmd(USART3,ENABLE);
		serialObjInit();
}

void USART3_IRQHandler(void)
{
	char inData;
	USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	inData = USART3->DR;

//	if( (inData == 0x55)&&(serial.lastInData == 0xff) )
//	{
//		serial.start |= 0x01;
//	}
//	serial.lastInData = inData;
//	// Start receive data
//	if( serial.start != 0 )
//	{
//		serial.buf[serial.n]=inData;
//		serial.n++;
//		serial.sum = (int)(0xff+0x55);
//		if( serial.n >10 )
//		{
//			for(serial.n=1;serial.n<10;serial.n++)
//			{
//				serial.sum += serial.buf[serial.n];
//			}
//			if( serial.sum == serial.buf[10] )
//			{
//				serial.ID = serial.buf[1];
//				serial.Flag = serial.buf[2];
//				serial.Index = (serial.buf[3]<<8) + serial.buf[4];
//				serial.SubIndex = serial.buf[5];
//				serial.Data = (serial.buf[6]<<24) + (serial.buf[7]<<16) + (serial.buf[8]<<8) + serial.buf[9];
//			}
//			CAN_send(serial.ID,serial.Flag,serial.Index,serial.SubIndex,serial.Data);
//			serial.n = 0;
//			serial.start = 0;
//		}
//	}
	
}
