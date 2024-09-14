#ifndef __AT32_OLED_H_
#define __AT32_OLED_H_

#include "at32f402_405.h"               // Device header

#define OLED_IIC_EN		0
#define OLED_SPI_EN		1

#define OLED_SOFT_IIC_EN		1
#define OLED_SOFT_SPI_EN		1
#define OLED_SH1107_EN		0  //0  表示为SSD1306即128*64的屏

#if OLED_SH1107_EN==1

#define OLED_WIDTH		128
#define OLED_HEIGHT		128

#else 

#define OLED_WIDTH		128
#define OLED_HEIGHT		64

#endif

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);


#endif

