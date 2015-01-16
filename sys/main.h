#ifndef _MAIN_H_
#define _MAIN_H_
#include "includes.h"

extern xTaskHandle ledTaskHandle;
extern xTaskHandle sciTaskHandle;
extern xTaskHandle CLITaskHandle;


void startTask ( void *pvParameters );
#endif
