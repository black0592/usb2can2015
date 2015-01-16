#include "spi.h"
#include "includes.h"


/*
*** Init the spi port, io clock and interrupt.
*** Pins:
			CSn		->	PA8
			MOSI	->	PB15
			MISO	-> 	PB14
			SCK		->	PB13
*** SPI2
*/
void SPI2_Init( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	/* Setup clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO + RCC_APB2Periph_GPIOA+ RCC_APB2Periph_GPIOB,ENABLE);
	
	/* Setup IO port */
	// CSn
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
	
	// MOSI and SCLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13+GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	// MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	/* Setup SPI2 */
	SPI_Cmd(SPI2, DISABLE);  
	SPI_I2S_DeInit(SPI2);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//Specifies the SPI unidirectional or bidirectional data mode.
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//Specifies the SPI operating mode.
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;//Specifies the SPI data size.
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//Specifies the serial clock steady state.
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; //Specifies the clock active edge for the bit capture.
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2,&SPI_InitStructure);
	//SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_RXNE,ENABLE);
	SPI_Cmd(SPI2,ENABLE);
	SPI_NSSInternalSoftwareConfig(SPI2,SPI_NSSInternalSoft_Set);
}

/**
  * @brief  Sends a data through the SPI interface and return the byte
  *   received from the SPI bus.
  * @param data : data to send.
  * @retval : The value of the received data.
  */
int SPI_SEND( int data )
{
	SPI_CS_LOW();
	
	/* Loop while DR register is not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	
  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI2, data);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
 
	SPI_CS_HIGH();
	
	/* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

int SPI_READ( int addr)
{
	SPI_CS_LOW();
	
	/* Loop while DR register is not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	
  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI2, addr);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
 
	SPI_CS_HIGH();
	
	vTaskDelay(1);

	SPI_CS_LOW();
	
	/* Loop while DR register is not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	
  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI2, addr);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
 
	SPI_CS_HIGH();	
	
	/* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

