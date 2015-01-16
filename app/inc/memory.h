#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "main.h"
#include "stm32f10x.h"
#include "current.h"

typedef struct
{
	PIFFTypeDef PIFF;
	int maxpwm;
}memoryType;

extern memoryType memory;


/* FLASK PAGE127 地址 *///中容量最后一页
#define PAGE127_ADDR 0X0801FC00

/* 存储起始地址 */
#define START_ADDR PAGE127_ADDR

/* Current Loop Parameters */
/* address for kcp */
#define KCP_ADDR (START_ADDR+4*1)

/* address for kci */
#define KCI_ADDR ( START_ADDR+4*2 )

/* address for kcff */
#define KCFF_ADDR ( START_ADDR+4*3 )

/* address for maxTorqeError */
#define MAXTERROR_ADDR (( START_ADDR+4*4 ))

/* address for maxTorqeError */
#define MAXPWM_ADDR (( START_ADDR+4*5 ))

void memoryRemberWord( uint32_t Address, uint32_t Data );
void memoryRemberHalfWord( uint32_t Address, uint16_t Data );
long memoryReadWord( uint32_t Address );
void memoryErasePage( uint32_t Page_Address );
void memoryRemberAll( memoryType* memory );
void initAllParameters( memoryType* memory );
#endif


