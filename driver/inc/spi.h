#ifndef _SPI_H_
#define _SPI_H_

#define SPI_CS_LOW() 	GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET)
#define SPI_CS_HIGH()	GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET)

void SPI2_Init( void );
int SPI_SEND( int data );

#endif
