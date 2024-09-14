#include "at32f402_405.h"               // Device header
#include "at32_rgb_animation.h"
#include "soft_timer.h"

const static RGB_Color_TypeDef liushui_table[][5] = 					
{
	 {
		 MAGENTA,MAGENTA_TO_RED_1,MAGENTA_TO_RED_2,MAGENTA_TO_RED_3,RED,
	 }, //1
	 {
		BLUE_TO_MAGENTA_3,MAGENTA,MAGENTA_TO_RED_1,MAGENTA_TO_RED_2,MAGENTA_TO_RED_3,
	 }, //2
	 {
		BLUE_TO_MAGENTA_2,BLUE_TO_MAGENTA_3,MAGENTA,MAGENTA_TO_RED_1,MAGENTA_TO_RED_2,
	 }, //3
	 {
		BLUE_TO_MAGENTA_1,BLUE_TO_MAGENTA_2,BLUE_TO_MAGENTA_3,MAGENTA,MAGENTA_TO_RED_1,
	 }, //4
	 {
		BLUE,BLUE_TO_MAGENTA_1,BLUE_TO_MAGENTA_2,BLUE_TO_MAGENTA_3,MAGENTA,
	 }, //5
	 {
		SKY_TO_BLUE_3,BLUE,BLUE_TO_MAGENTA_1,BLUE_TO_MAGENTA_2,BLUE_TO_MAGENTA_3,
	 }, //6
	 {
		SKY_TO_BLUE_2,SKY_TO_BLUE_3,BLUE,BLUE_TO_MAGENTA_1,BLUE_TO_MAGENTA_2,
	 }, //7 
	 {
		SKY_TO_BLUE_1,SKY_TO_BLUE_2,SKY_TO_BLUE_3,BLUE,BLUE_TO_MAGENTA_1,
	 }, //8	 
	 {
		SKY,SKY_TO_BLUE_1,SKY_TO_BLUE_2,SKY_TO_BLUE_3,BLUE,
	 }, //9	 
	 
};

static void LiushuiMap_to_rgb(RGB_Color_TypeDef* table)
{
	uint8_t i=0,temp=0;//Pixel_NUM<=255可用
			for(i=0;i<Pixel_NUM;i++)
			{				
				temp=i%10;
				if(temp<5) 
				{
					if(i<=14) 			{RGB_SetOne_Color(i-1,table[temp]);}	
					else if(i<=26)	{RGB_SetOne_Color(i-2,table[temp]);}
					else if(i<=29)	{RGB_SetOne_Color(i-3,table[temp]);}
//				RGB_SetOne_Color(i,table[temp]);
				}
				else			 
				{
					if(i<=14) 			{RGB_SetOne_Color(i-1,table[9-temp]);}	
					else if(i<=26)	{RGB_SetOne_Color(i-2,table[9-temp]);}
					else if(i<=29)	{RGB_SetOne_Color(i-3,table[9-temp]);}
//				RGB_SetOne_Color(i,table[9-temp]);
				}
			}		
}




void Map_Setone_color(uint8_t i,uint8_t j,RGB_Color_TypeDef rgb_color)
{
		uint8_t pos=0;
		if(!(i%2))  
		{
			pos=(i/2)*10+j;			
			
		}//i为偶
		else				
		{
			pos=((i-1)/2)*10+(9-j);
		}//i为奇
//		RGB_SetOne_Color(pos,rgb_color);
	if(pos<=14) 									{RGB_SetOne_Color((uint8_t)(pos-1),rgb_color);}	
	if(pos>=16&&pos<=26)					{RGB_SetOne_Color(pos-2,rgb_color);}
	if(pos>=28&&pos<=29)					{RGB_SetOne_Color(pos-3,rgb_color);}
}


static void Liushui0(void)
{
	LiushuiMap_to_rgb((RGB_Color_TypeDef*)liushui_table[0]);
}

static void Liushui1(void)
{
	LiushuiMap_to_rgb((RGB_Color_TypeDef*)liushui_table[1]);
}

static void Liushui2(void)
{
	LiushuiMap_to_rgb((RGB_Color_TypeDef*)liushui_table[2]);
}

static void Liushui3(void)
{
	LiushuiMap_to_rgb((RGB_Color_TypeDef*)liushui_table[3]);
}

static void Liushui4(void)
{
	LiushuiMap_to_rgb((RGB_Color_TypeDef*)liushui_table[4]);
}

static void Liushui5(void)
{
	LiushuiMap_to_rgb((RGB_Color_TypeDef*)liushui_table[5]);
}

static void Liushui6(void)
{
	LiushuiMap_to_rgb((RGB_Color_TypeDef*)liushui_table[6]);
}

static void Liushui7(void)
{
	LiushuiMap_to_rgb((RGB_Color_TypeDef*)liushui_table[7]);
}

static void Liushui8(void)
{
	LiushuiMap_to_rgb((RGB_Color_TypeDef*)liushui_table[8]);
}

static void Liushui_end(void)
{
	RGB_SetMore_Color(0,Pixel_NUM-1,BLACK);
}

void Liushui(void)
{
	SoftTimerActiveEvent(Liushui0, 0, 0);
	SoftTimerActiveEvent(Liushui1, 50*1, 0);
	SoftTimerActiveEvent(Liushui2, 50*2, 0);
	SoftTimerActiveEvent(Liushui3, 50*3, 0);
	SoftTimerActiveEvent(Liushui4, 50*4, 0);
	SoftTimerActiveEvent(Liushui5, 50*5, 0);
	SoftTimerActiveEvent(Liushui6, 50*6, 0);
	SoftTimerActiveEvent(Liushui7, 50*7, 0);
	SoftTimerActiveEvent(Liushui8, 50*8, 0);
	SoftTimerActiveEvent(Liushui_end, 50*9, 0);
}

uint8_t breath_pos=0;
uint8_t b_flag=1,rgbbreath_flag;

void light_breathing(void)
{
	if(breath_pos<2)  {b_flag=1;rgbbreath_flag++;rgbbreath_flag%=5;}
	if(breath_pos>200)	b_flag=0;
	if(b_flag)  {breath_pos=breath_pos+2;}
	else  			{breath_pos=breath_pos-2;}//breath_pos属于[1,200]
	switch (rgbbreath_flag)
  {
  	case 0:
			RGB_SetMore_Color(0,Pixel_NUM-1,(RGB_Color_TypeDef){breath_pos,breath_pos,breath_pos});
  		break;
  	case 1:
			RGB_SetMore_Color(0,Pixel_NUM-1,(RGB_Color_TypeDef){breath_pos,0,breath_pos});
  		break;
  	case 2:
			RGB_SetMore_Color(0,Pixel_NUM-1,(RGB_Color_TypeDef){0,breath_pos,breath_pos});
  		break;
  	case 3:
			RGB_SetMore_Color(0,Pixel_NUM-1,(RGB_Color_TypeDef){0,0,breath_pos});
  		break;
  	case 4:
			RGB_SetMore_Color(0,Pixel_NUM-1,(RGB_Color_TypeDef){0,breath_pos,0});
  		break;
  	default:
			RGB_SetMore_Color(0,Pixel_NUM-1,(RGB_Color_TypeDef){breath_pos,0,0});
  		break;
  }
	
}
//uint8_t RGB_Mode_flag;
//void RGB_Mode(void)
//{
//	if(RGB_Mode_flag==0)//开机
//	{
//		Liushui();
//		RGB_Mode_flag=1;
//	}
//	if(RGB_Mode_flag==1)//正常
//	{
//		;
//	}
//	if(RGB_Mode_flag==2)//空闲
//	{
//		light_breathing();
//	}

//}
