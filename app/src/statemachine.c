#include "statemachine.h"
#include "includes.h"
#include "canopen.h"

void DriveInitialize( void )
{
	
}
void DriveSelftest( void )
{

}

void ActivateCommunication( void )
{

}

void RunStateMachine( void )
{
	int state = Checkstatus(StatusWord.Param);
	
	switch( state )
	{
		case STATE_START:
			//Event: reset
			//Action: The drive self-tests and/or initialized successfully
			DriveInitialize();
			DriveSelftest();
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.		
			break;
		case STATE_NOT_READY_TO_SWITCH_ON:
			//Event: The drive has self-tested and/or initialized successfully
			//Action: Activate communication
			ActivateCommunication();
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.			
			
			break;
		case STATE_SWITCH_ON_DISABLED:
			//Event: 'Shutdown' command received from host
			//Action: None
		
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.	
			break;
		case STATE_READY_TO_SWITCH_ON:
			//Event: 'Switch On' Command received from host
			//Action: The power section is switched on if it is not already switched on.
		
			//Event: 'Quick Stop' and 'Disable Voltage' command received from host
			//Action: None.
		
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.			
			break;
		case STATE_SWITCHED_ON:
			//Event: 'Enable Operation' command received frim host.
			//Action: The drive operation will be disabled.
			
			//Event: 'Shutdown' command received from host.
			//Action: The power section is switched off.
			
			//Event: 'Disable Voltage' or 'Quick Stop' command received from host.
			//Action: The power section is switched off immediately, and the motor is free to rotate if unbraked. 
		
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.		
			break;		
		case STATE_OPERATION_ENABLED:
			//Event: 'Disable Operation' command received from host.
			//Action: The drive operation will be disabled.
		
			//Event: 'Shutdown' command received from host.
			//Action: The power section is switched off immediately, and the motor is free to rotate if unbraked. 
		
			//Event: 'Disable Voltage' command received from host.
			//Action: The power section is switched off immediately, and the motor is free to rotate if unbraked. 
			
			//Event:	'Quick Stop' command received from host.
			//Action:	The quick stop function is executed.
			
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.
			break;
		case STATE_QUICK_STOP_ACTIVE:
			//Event: 'Quick Stop' is completed or 'Disable Voltage' command received from host.
			//Action: The Power section if switched off.
		
			//Event: 'Enable Operation' command received from host.
			//Action: The drive function is enabled.
		
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.		
			break;
		case STATE_FAULT_REACTION_ACTIVE:
			//Event: The fault reaction is completed.
			//Action: The drive fuction is disabled. The power section may be switched off.

			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.			
			break;
		case STATE_FAULT:
			//Event: 'Fault Reset' command received from host.
			//Action: A reset of the fault condition is carried out if no fault exists currently on the drive.
			//				Afer leaving the state Fault the Bit 'Fault Reset' of the controlword has to be cleared by the host.
		
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.	
			break;
		default: 
			break;
	}
	
}

void DriveTask( void *pvParameters )
{
	for(;;)
	{
		RunStateMachine();
		vTaskDelay(1);
	}
}


