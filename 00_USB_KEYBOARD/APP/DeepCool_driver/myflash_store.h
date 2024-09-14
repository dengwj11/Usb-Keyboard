#ifndef __MYFLASH_STORE_H_
#define __MYFLASH_STORE_H_

#include "at32f402_405.h"               // Device header
#define MYFLASH_SECTOR_ADDR   0x0803F800   //256KB的最后一个扇区

extern uint8_t Store_Data[];
void Store_Init(void);
void Store_Save(void);
void Store_Clear(void);

#endif
