#ifndef _ENCODER_H_
#define _ENCODER_H_

#define ENCODER TIM3->CNT


/*�����������ṹ��*/
typedef struct _encoderReadType
{
	int A; //���ٶ�
	long V; //�ٶ�
	long S; //λ��
	int P;	//���λ��
}encoderReadType;

///*�����������Ķ�ȡ���ݵĽṹ��*/
extern encoderReadType Encoder;

void Encoder_Configuration(void);
void encoderTask ( void *pvParameters );

#endif
