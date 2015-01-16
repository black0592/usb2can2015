#include "canopen.h"
#include "includes.h"
#include "can.h"
//#include "memory.h"


int operation_mode;
 
COBJ ControlWord={CONTROLWORD_INDEX,0,CONTROLWORD_SWITCH_ON};//上电 即打开
COBJ StatusWord={STATUSWORD_INDEX,0,0};
COBJ TargetTorque={TARGETTORQUE_INDEX,0,0};
COBJ ModesofOperation={MODES_OF_OPERATION_INDEX,0,VELOCITY_MODE};//速度模式 开环
COBJ ModesofOperationDisplay={MODES_OF_OPERATION_DISPLAY_INDEX,0,0};
COBJ MotorType={MOTORTYPE_INDEX,0,MOTORTYPE_BDCMOTOR};//直流有刷电机
COBJ CurrentLoopKcp={CURRENTLOOPPID_INDEX,CURRENTLOOPPID_KCP_SUBINDEX,1};
COBJ CurrentLoopKci={CURRENTLOOPPID_INDEX,CURRENTLOOPPID_KCI_SUBINDEX,0};
COBJ CurrentLoopKcff={CURRENTLOOPPID_INDEX,CURRENTLOOPPID_KCFF_SUBINDEX,0};


//void canopen_statemachine(void)
//{	
//	if(MotorType.Param==MOTORTYPE_BDCMOTOR)
//	{
//		adc=-adc2;
//	}
//	else if(MotorType.Param==MOTORTYPE_TPMBLMOTOR)
//	{
//		switch(hallValue)
//		{
//			case 5:
//					adc=-adc1;
//				break;
//			case 1:
//					adc=-(adc1-adc2)/2;
//				break;
//			case 3:
//					adc=adc2;
//				break;
//			case 2:
//					adc=adc1;
//				break;
//			case 6:
//					adc=(adc1-adc2)/2;
//				break;
//			case 4:
//					adc=-adc2;
//				break;
//			default:break;
//		}

//	}
//	

//	operation_mode=ModesofOperation.Param;
//	if(((ControlWord.Param))==CONTROLWORD_SWITCH_ON)
//	{
//		switch(operation_mode)
//		{
//			case -1://PWM mode
//				
//				pwm=ADC1_Value[2]/3;
//				break;
//			case PROFILE_POSITION_MODE://Profile Position Mode
//				break;
//			case VELOCITY_MODE://Velocity Mode
//				pwm=(ADC1_Value[2]-2048)/3;
//				break;
//			case PROFILE_VELOCITY_MODE:
//				break;
//			case TORQUE_PROFILE_MODE://Torque Profile Mode
//				pwm=DoPIFF(adc,(TargetTorque.Param),&(memory.PIFF));
//				break;
//			case HOMING_MODE:
//				break;
//			case INTERPOLATED_POSITION_MODE:
//				break;
//			default:
//				break;
//		}
//		if(pwm>memory.maxpwm)
//			pwm=memory.maxpwm;
//		if(pwm<-memory.maxpwm)
//			pwm = -memory.maxpwm;		

//		PWM(hallValue,pwm);		
//	}
//	else if(ControlWord.Param == 0x02)//for testing
//	{

//	}
//	else
//	{
//		TIM_SetCompare1(TIM1,0);//
//		TIM_SetCompare2(TIM1,0);//
//		TIM_SetCompare3(TIM1,0);//
//		GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
//		GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
//		GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);		
//	}		

//}
/* Device control commands */
//Shutdown
void Shutdown( void )
{
	ControlWord.Param &= ~CONTROLWORD_SWITCH_ON;
	ControlWord.Param |= CONTROLWORD_ENABLE_VOLTAGE;
	ControlWord.Param |= CONTROLWORD_QUICK_STOP;
	
	ControlWord.Param &= ~CONTROLWORD_FAULT_RESET;
	
}
//Switch on
void Switchon( void )
{
	ControlWord.Param |= CONTROLWORD_SWITCH_ON;
	ControlWord.Param |= CONTROLWORD_ENABLE_VOLTAGE;
	ControlWord.Param |= CONTROLWORD_QUICK_STOP;
	ControlWord.Param &= ~CONTROLWORD_ENABLE_OPERATION;
	ControlWord.Param &= ~CONTROLWORD_FAULT_RESET;
	
}
//Switch on2
void Switchon2( void )
{
	ControlWord.Param |= CONTROLWORD_SWITCH_ON;
	ControlWord.Param |= CONTROLWORD_ENABLE_VOLTAGE;
	ControlWord.Param |= CONTROLWORD_QUICK_STOP;
	ControlWord.Param |= CONTROLWORD_ENABLE_OPERATION;
	ControlWord.Param &= ~CONTROLWORD_FAULT_RESET;
}
//Disable voltage
void Disablevoltage( void )
{
	//ControlWord.Param |= CONTROLWORD_SWITCH_ON;
	ControlWord.Param &= ~CONTROLWORD_ENABLE_VOLTAGE;
	//ControlWord.Param |= CONTROLWORD_QUICK_STOP;
	//ControlWord.Param |= CONTROLWORD_ENABLE_OPERATION;
	ControlWord.Param &= ~CONTROLWORD_FAULT_RESET;	
}
//Quick stop
void Quickstop( void )
{
	//ControlWord.Param |= CONTROLWORD_SWITCH_ON;
	ControlWord.Param |= CONTROLWORD_ENABLE_VOLTAGE;
	ControlWord.Param &= ~CONTROLWORD_QUICK_STOP;
	//ControlWord.Param |= CONTROLWORD_ENABLE_OPERATION;
	ControlWord.Param &= ~CONTROLWORD_FAULT_RESET;
}
//Disable operation
void Disableoperation( void )
{
	ControlWord.Param |= CONTROLWORD_SWITCH_ON;
	ControlWord.Param |= CONTROLWORD_ENABLE_VOLTAGE;
	ControlWord.Param |= CONTROLWORD_QUICK_STOP;
	ControlWord.Param &= ~CONTROLWORD_ENABLE_OPERATION;
	ControlWord.Param &= ~CONTROLWORD_FAULT_RESET;
}
//Enable operation
void Enableoperation( void )
{
	ControlWord.Param |= CONTROLWORD_SWITCH_ON;
	ControlWord.Param |= CONTROLWORD_ENABLE_VOLTAGE;
	ControlWord.Param |= CONTROLWORD_QUICK_STOP;
	ControlWord.Param |= CONTROLWORD_ENABLE_OPERATION;
	ControlWord.Param &= ~CONTROLWORD_FAULT_RESET;
}
//Fault reset
void Faultreset( void )
{
	//ControlWord.Param |= CONTROLWORD_SWITCH_ON;
	//ControlWord.Param |= CONTROLWORD_ENABLE_VOLTAGE;
	//ControlWord.Param |= CONTROLWORD_QUICK_STOP;
	//ControlWord.Param |= CONTROLWORD_ENABLE_OPERATION;
	ControlWord.Param |= CONTROLWORD_FAULT_RESET;
}



/* Check status */
/* Return the state */
int Checkstatus( int status )
{
	int state=STATE_START;//
	if((status&0x004f)==0x0000)//XXXX XXXX X1XX 0000
	{
		state=STATE_NOT_READY_TO_SWITCH_ON;
	}
	else if((status&0x004f)==0x0040)
	{
		state=STATE_SWITCH_ON_DISABLED;
	}
	else if((status&0x006f)==0x0021)
	{
		state=STATE_READY_TO_SWITCH_ON;
	}
	else if((status&0x006f)==0x0023)
	{
		state=STATE_SWITCHED_ON;
	}
	else if((status&0x006f)==0x0027)
	{
		state=STATE_OPERATION_ENABLED;
	}
	else if((status&0x006f)==0x0007)
	{
		state=STATE_QUICK_STOP_ACTIVE;
	}
	else if((status&0x004f)==0x004F)
	{
		state=STATE_FAULT_REACTION_ACTIVE;
	}
	else if((status&0x004f)==0x0048)
	{
		state=STATE_FAULT;
	}
	
	return state;
}

/* Modes of Operation */
//mode can be found from MODES_OF_OPERATION_INDEX
void SetOperationMode( int mode )
{
	ModesofOperation.Param = mode;
	ModesofOperationDisplay.Param = mode;
}

/* Set Motor Type */
//Motor type can be found from MOTORTYPE_INDEX
void SetMotorType( int type )
{
	MotorType.Param = type;
}
