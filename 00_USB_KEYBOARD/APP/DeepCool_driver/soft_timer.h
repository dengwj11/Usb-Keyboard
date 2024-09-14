#ifndef __SOFT_TIMER_H__
#define __SOFT_TIMER_H__

#include "stdio.h"

//#define SCH_TASKS (1)
#define MAX_SOFT_TIMER_AMOUNT 32
#define SOFT_TIMER_ERROR                                        0x01
#define SOFT_TIMER_OK                                           0x00

typedef struct
{
	void (* event)(void);
	unsigned short delay;
	unsigned short period;
	unsigned short runme;
}SOFT_TIMER_EVENT_STRUCT;

void SoftTimerInit(void);
void SoftTimerHandler(void);
unsigned char SoftTimerActiveEvent(void (* pFunction)(void), const unsigned short delay, const unsigned short period);
void SoftTimerISR(void);
void SoftTimerDeleteEvent(const unsigned char index);

#endif

