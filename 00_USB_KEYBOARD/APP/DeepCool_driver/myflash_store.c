#include "myflash_store.h"             
#include "myflash.h"  


uint8_t Store_Data[512];

void Store_Init(void)//FLASH-->Store_Data[512]
{
	if(MyFlash_ReadByte(MYFLASH_SECTOR_ADDR) != 0xA5)
	{
			MyFlash_EraseSector(MYFLASH_SECTOR_ADDR);
			 MyFlash_ProgramByte(MYFLASH_SECTOR_ADDR,0xA5);
			for(uint16_t i=1;i<512;i++)
			{
//				 MyFlash_ProgramHalfWord(MYFLASH_SECTOR_ADDR+i*2,0x0000);
				 MyFlash_ProgramByte(MYFLASH_SECTOR_ADDR+i,0x00);
			}
	}
  for(uint16_t i=0;i<512;i++)
	{
//			Store_Data[i] = MyFlash_ReadHalfWord(MYFLASH_SECTOR_ADDR+i*2);
			Store_Data[i] =	MyFlash_ReadByte(MYFLASH_SECTOR_ADDR+i);
	}
}

void Store_Save(void)//Store_Data[512]-->FLASH
{
		MyFlash_EraseSector(MYFLASH_SECTOR_ADDR);
		 for(uint16_t i=0;i<512;i++)
		{
//			MyFlash_ProgramHalfWord(MYFLASH_SECTOR_ADDR+i*2,Store_Data[i] );
			MyFlash_ProgramByte(MYFLASH_SECTOR_ADDR+i,Store_Data[i]);
		}
}


void Store_Clear(void)
{
		for(uint16_t i=1;i<512;i++)
		{
			Store_Data[i] = 0x00;
		}
		Store_Save();
}



