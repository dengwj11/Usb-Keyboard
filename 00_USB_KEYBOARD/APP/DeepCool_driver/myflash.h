#ifndef __MYFLASH_H_
#define __MYFLASH_H_

#include "at32f402_405.h"               // Device header


uint32_t MyFlash_ReadWord(uint32_t Address);
uint16_t MyFlash_ReadHalfWord(uint32_t Address);
uint8_t MyFlash_ReadByte(uint32_t Address);
void MyFlash_EraseAllpages(void);//È«²Á³ý£¬É÷ÓÃ£¡
void MyFlash_EraseSector(uint32_t Sectoraddress);
void MyFlash_ProgramWord(uint32_t Address,uint32_t Data);
void MyFlash_ProgramHalfWord(uint32_t Address,uint16_t Data);
void MyFlash_ProgramByte(uint32_t Address,uint8_t Data);

#endif

