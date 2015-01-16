#ifndef _SERIAL_H_
#define _SERIAL_H_
typedef struct
{
	int start;
	char lastInData;
	char buf[12];
	int n;
	char sum;
	int ID;
	int Flag;
	int Index;
	int SubIndex;
	int Data;
}serialObj;
void serialPortInit( void );
#endif
