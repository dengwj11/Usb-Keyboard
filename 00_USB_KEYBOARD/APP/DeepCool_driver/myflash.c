#include "myflash.h"  
#include "at32f402_405_flash.h"
uint32_t MyFlash_ReadWord(uint32_t Address)
{
			return *((__IO uint32_t *)(Address));
}

uint16_t MyFlash_ReadHalfWord(uint32_t Address)
{
			return *((__IO uint16_t *)(Address));
}

uint8_t MyFlash_ReadByte(uint32_t Address)
{
			return *((__IO uint8_t *)(Address));
}

void MyFlash_EraseAllpages(void)//È«²Á³ý£¬É÷ÓÃ£¡
{
	flash_unlock();
	flash_internal_all_erase();
	flash_lock();
}

void MyFlash_EraseSector(uint32_t Sectoraddress)
{
	flash_unlock();
	flash_sector_erase(Sectoraddress);
	flash_lock();

}	

void MyFlash_ProgramWord(uint32_t Address,uint32_t Data)
{
	flash_unlock();
	flash_word_program(Address,Data);
	flash_lock();	
}


void MyFlash_ProgramHalfWord(uint32_t Address,uint16_t Data)
{
	flash_unlock();
	flash_halfword_program(Address, Data);
	flash_lock();	
}


void MyFlash_ProgramByte(uint32_t Address,uint8_t Data)
{
	flash_unlock();
	flash_byte_program(Address, Data);
	flash_lock();	
}



