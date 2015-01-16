#include "can.h"
#include "includes.h"
#include "led.h"
int temp =0;

CanTxMsg TxMessage;
CanRxMsg RxMessage;

void CAN_Configuration(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	//CAN_FilterInitTypeDef  CAN_FilterInitStructure;		
	GPIO_InitTypeDef 	   GPIO_InitStructure;
	NVIC_InitTypeDef   		NVIC_InitStructure;

	/* Enable the clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	/* Enable IO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* CAN Port Remap */
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);

	/* Configurate CAN */
	CAN_DeInit(CAN1);
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=ENABLE;
//	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
 	CAN_InitStructure.CAN_Mode=	CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler=4;
	CAN_Init(CAN1, &CAN_InitStructure);
	
//	CAN_FilterInitStructure.CAN_FilterNumber=0;
//	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
//	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
//	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
//	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
//	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
//	CAN_FilterInit(&CAN_FilterInitStructure);

	/* Enable the CAN1 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn,15);
	/* Enable CAN RX Interupt */
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}

/* Send CAN Object */
void CAN_send(int id, int flag, int index,int subindex, int param)		
{
//  CanTxMsg TxMessage;
  TxMessage.StdId=id;
  TxMessage.RTR=CAN_RTR_DATA;
  TxMessage.IDE=CAN_ID_STD;
  TxMessage.DLC=8;
  TxMessage.Data[0]=flag;
  TxMessage.Data[1]=index;
  TxMessage.Data[2]=index>>8;
  TxMessage.Data[3]=subindex;
  TxMessage.Data[4]=param;
  TxMessage.Data[5]=param>>8;
  TxMessage.Data[6]=param>>16;
  TxMessage.Data[7]=param>>24;
	CAN_Transmit(CAN1, &TxMessage);

}
void ReceiveData(void)
{
//		COBJ RX;
//		int Index;
		RxMessage.StdId=0x00;
    RxMessage.IDE=CAN_ID_STD;
    RxMessage.DLC=0;
    RxMessage.Data[0]=0x00;
    RxMessage.Data[1]=0x00;
    CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
////		RX.COB_ID =RxMessage.StdId;         			//  CAN报文IDelmo->ulNodeID
////		
////		RX.Specifier=RxMessage.Data[0];            //  CAN报文前缀
//		RX.Index = RxMessage.Data[1] + (RxMessage.Data[2]<<8);
//		RX.SubIndex= RxMessage.Data[3];                //  发送SubIndex
//		RX.Param=RxMessage.Data[4]+(RxMessage.Data[5]<<8)+(RxMessage.Data[6]<<16)+(RxMessage.Data[7]<<24);
//		if(RxMessage.Data[0] == CAN_SPECIFIER)
//		{
//			Index=(RxMessage.Data[1] + (RxMessage.Data[2]<<8));
//			switch( Index )
//			{
//				case CONTROLWORD_INDEX:
//					ControlWord.Param = RX.Param;
//					break;
//				case MODES_OF_OPERATION_INDEX:
//					ModesofOperation.Param=RX.Param;
//					break;
//				case MOTORTYPE_INDEX:
//					MotorType.Param = RX.Param;
//					break;
//				case TARGETTORQUE_INDEX:
//					TargetTorque.Param=RX.Param;
//				default: 
//					break;
//			}
//		}
}


/* CAN RX Interupt Handle */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0) == SET)
	{
		CAN_ClearFlag(CAN1,CAN_FLAG_FMP0);
		ReceiveData();
		temp++;
	}
	ReceiveData();
	temp++;
}

/* CAN Test Task */
void canTestTask( void *pvParameters )
{
	int param = 0;
	for(;;)
	{
		vTaskDelay(2000);
		CAN_send(0x0F,0x02,0x03,0x04,param);
		param++;
	}
}

