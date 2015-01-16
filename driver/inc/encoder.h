#ifndef _ENCODER_H_
#define _ENCODER_H_

#define ENCODER TIM3->CNT


/*编码器读数结构体*/
typedef struct _encoderReadType
{
	int A; //加速度
	long V; //速度
	long S; //位置
	int P;	//相对位置
}encoderReadType;

///*三个编码器的读取数据的结构体*/
extern encoderReadType Encoder;

void Encoder_Configuration(void);
void encoderTask ( void *pvParameters );

#endif
