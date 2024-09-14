#ifndef __AT32_RGB_ANIMATION_H_
#define __AT32_RGB_ANIMATION_H_

#include "at32_ws2812_rgb.h"
#define LIUSHUI_ENDTIME		50*9

//extern uint8_t RGB_Mode_flag;
void Map_Setone_color(uint8_t i,uint8_t j,RGB_Color_TypeDef rgb_color);
void Liushui(void);
void light_breathing(void);
//void RGB_Mode(void);
#endif

