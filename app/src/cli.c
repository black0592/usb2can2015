/*
================================================================================
* File Name                 :   cli.c
* Copyright                 :   2011-2013
* 
* CPU                       :   STM32F103
* RTOS                      :   FreeRTOS
*
* Create Date               :   2012/12/1
* Author/Company            :   Zhan Yubo/UESTC
* Abstract Description      :   Configuration and Applications of Command Line Interface.
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*	NO.1		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/

#include "includes.h"
#include "cli.h"
#include "sci.h"
//#include "led.h"
//#include "pid.h"
//#include "math.h"
//#include "hall.h"
#include <string.h>
//#include "check.h"
//#include "test.h"
//#include "main.h"
//#include "current.h"
//#include "memory.h"
//#include "display.h"
//#include "canopen.h"
//#include "dma.h"

int feedback=FEEDBACK_NONE;//YES NO NONE


/****
	*	calc the length of command in a string
	*/
int cmdlen(char *string)
{
	int len = 0;
	while( (*string != ' ')&&( *string != '\0' ))
	{
		string++;
		len++;
	}
	return len;	
}

/***
	* change string to float number
*/
float string2number( const char *string )
{
	int length = 0;
	long dot = 0;
	int i=0;
	float result=0;
	int k=0;
	length = strlen(string);
	
	for(i=0;i<length;i++)
	{
		if(string[i]=='.')
		{
			dot = length-i;
		}
	}

	for(i=length-1;i>=0;i--)
	{
		k = length-dot-1-i;
		if(k<0)
			k=k+1;
		switch(string[i])
		{
			case '0': result += 0;break;
			case '1': result += 1*pow(10,k);break;
			case '2': result += 2*pow(10,k);break;
			case '3': result += 3*pow(10,k);break;
			case '4': result += 4*pow(10,k);break;
			case '5': result += 5*pow(10,k);break;
			case '6': result += 6*pow(10,k);break;
			case '7': result += 7*pow(10,k);break;
			case '8': result += 8*pow(10,k);break;
			case '9': result += 9*pow(10,k);break;
			case '-': result = result*(-1);break;
			case '.': break;
			default: result = 0;sciSendString("\r\nInvalid Parameter!!",bMessage);return 0;
		}
	}	
	return result;
}
/***
	* check the string are numbers or not
	* if numbers return 1
	* else return 0
*/
int checkNumbers(const char *string)
{
	int length = 0;
	int i=0;
	length = strlen(string);

	for(i=length-1;i>=0;i--)
	{
		switch(string[i])
		{
			case '0': break;
			case '1': break;
			case '2': break;
			case '3': break;
			case '4': break;
			case '5': break;
			case '6': break;
			case '7':	break;
			case '8': break;
			case '9': break;
			case '-': break;
			case '.': break;
			default: return 0;
		}
	}	
	return 1;
}

void createCommandList(commandListType* commandList)
{
	commandList->head = 0;
	commandList->tail = commandList->head;
}

void addCommandToList(commandType* command,commandListType* commandList)
{
	
}

void findCommandInList(char* string)
{

	commandType command;
	int i=0;
	char stringBuf[100];
	
	/* calc the length of the cmd */
	command.NameLength = cmdlen(string);

	/* calc the length of the whole string */
	command.StringLength = strlen(string);

	/* calc the length of parameters */
	if(command.StringLength-command.NameLength>0)
		command.ParameterLength = command.StringLength-command.NameLength-1;
	else
		command.ParameterLength = 0;

	/* copy the cmd string from the whole string */
	for(i=0;i<command.NameLength;i++)
	{
		command.Name[i]=string[i];
	}
	for(i=i;i<MaxCmdLength;i++)
	{
		command.Name[i]='\0';
	}

	/* if there is parameter, copy it to parameter string */
	if(command.ParameterLength!=0)
	{
		for(i=command.NameLength+1;i<command.StringLength;i++)
		{
			command.Parameter[i-command.NameLength-1]=string[i];
		}
		for(i=i;i-command.NameLength-1<MaxParameterLength;i++)
		{
			command.Parameter[i-command.NameLength-1]='\0';
		}
	}
	else
	for(i=0;i<MaxParameterLength;i++)
	{
		command.Parameter[i]='\0';
	}
	
	/* if the Parameters are numbers, chage them to a float number*/
	if(checkNumbers(command.Parameter))
	{
		command.parameter = string2number(command.Parameter);
	}
	/* DISPLAY */
		if(strcmp(command.Name,"display")==0)
	{
		if(command.ParameterLength!=0)
		{
			if(command.parameter)
			{
				//xTaskCreate( displayTask, "display", configMINIMAL_STACK_SIZE, NULL, 5, &displayTaskHandle );
			}
			else
			{
				//vTaskDelete(displayTaskHandle);
			}
		}		
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(int2String(command.parameter,stringBuf),bMessage);		
	}	
	/* OTHERS */
	else
	{
		sciSendString("\r\n",bMessage);
		sciSendString("Unknown Command ",bMessage);
	}	
	
	
}



/*========================== END OF FILE =======================================*/
