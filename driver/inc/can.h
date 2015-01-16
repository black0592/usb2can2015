#ifndef _CAN_H_
#define _CAN_H_

extern void CAN_Configuration(void);
extern void CAN_ErrRecConfig(void);
void CAN_send(int id, int flag, int index,int subindex, int param)	;
void canTestTask( void *pvParameters );
#endif


