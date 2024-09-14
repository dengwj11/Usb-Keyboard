#ifndef __AT32_WS2812_RGB__
#define __AT32_WS2812_RGB__

#include "at32f402_405.h"               // Device header
//#include "at32f425.h"   


//RGB灯带控制:
//PWM输出io:TMR2_CH1--->PA0
//PWM输出io:TMR3_CH1--->PA6
#define RGB_PIN                         GPIO_PINS_0
#define RGB_PIN_SOURCE                  GPIO_PINS_SOURCE0
#define RGB_PIN_MUX                     GPIO_MUX_1//!!!!!重要
#define RGB_GPIO                        GPIOA
#define RGB_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK

#define RGB_TMR             		  			TMR2
#define RGB_TMR_CLK               		  CRM_TMR2_PERIPH_CLOCK
#define RGB_TMR_CHANNEL               	TMR_SELECT_CHANNEL_1

#define RGB_DMA                					DMA1
#define RGB_DMA_CLK                			CRM_DMA1_PERIPH_CLOCK
#define RGB_DMA_CHANNEL                	DMA1_CHANNEL5
#define RGB_DMA_FLEX_CHANNEL            DMA1MUX_CHANNEL5
#define RGB_DMAMUX_OVERFLOW            	DMAMUX_DMAREQ_ID_TMR2_OVERFLOW
#define RGB_DMA_Channel_IRQn            DMA1_Channel5_IRQn

	//单个LED的颜色控制结构体
	typedef struct
	{
		uint8_t R8;
		uint8_t G8;
		uint8_t B8;
	}RGB_Color_TypeDef;

 
#define Pixel_NUM 50  //键盘6*5 最多需要30个灯
	//对于at32k8t7  LED数量宏定义<800,最多可控制799，占用RAM为19.992KB
	
#define BLACK    (RGB_Color_TypeDef){0,0,0}    //无颜色	

#define WHITE    		(RGB_Color_TypeDef){255,255,255} //白色
#define WHITE_7     (RGB_Color_TypeDef){240,240,240} //白色7
#define WHITE_6     (RGB_Color_TypeDef){200,200,200} //白色6
#define WHITE_5     (RGB_Color_TypeDef){100,100,100} //白色5
#define WHITE_4     (RGB_Color_TypeDef){60,60,60}		 //白色4
#define WHITE_3     (RGB_Color_TypeDef){30,30,30} 	 //白色3
#define WHITE_2     (RGB_Color_TypeDef){20,20,20} 	 //白色2
#define WHITE_1     (RGB_Color_TypeDef){10,10,10} 	 //白色1


#define RED      	(RGB_Color_TypeDef){255,0,0} 
#define RED_7     (RGB_Color_TypeDef){240,0,0} //红色7
#define RED_6     (RGB_Color_TypeDef){200,0,0} //红色6
#define RED_5     (RGB_Color_TypeDef){100,0,0} //红色5
#define RED_4     (RGB_Color_TypeDef){60,0,0} //红色4
#define RED_3     (RGB_Color_TypeDef){30,0,0} //红色3
#define RED_2     (RGB_Color_TypeDef){20,0,0}  //红色2
#define RED_1     (RGB_Color_TypeDef){10,0,0}  //红色1

#define RED_TO_YELLOW_1     (RGB_Color_TypeDef){255,100,0}  
#define RED_TO_YELLOW_2     (RGB_Color_TypeDef){255,200,0}  
#define RED_TO_YELLOW_3     (RGB_Color_TypeDef){255,200,0}  


#define YELLOW   		 (RGB_Color_TypeDef){255,255,0}  //黄色
#define YELLOW_7     (RGB_Color_TypeDef){240,240,0} //黄色7
#define YELLOW_6     (RGB_Color_TypeDef){200,200,0} //黄色6
#define YELLOW_5     (RGB_Color_TypeDef){100,100,0} //黄色5
#define YELLOW_4     (RGB_Color_TypeDef){60,60,0} //黄色4
#define YELLOW_3     (RGB_Color_TypeDef){30,30,0} //黄色3
#define YELLOW_2     (RGB_Color_TypeDef){20,20,0} 	  //黄色2
#define YELLOW_1     (RGB_Color_TypeDef){10,10,0} 	  //黄色1

#define YELLOW_TO_GREEN_1     (RGB_Color_TypeDef){200,255,0}  
#define YELLOW_TO_GREEN_2     (RGB_Color_TypeDef){140,255,0}  
#define YELLOW_TO_GREEN_3     (RGB_Color_TypeDef){70,255,0}  

#define GREEN   	  (RGB_Color_TypeDef){0,255,0} //绿色
#define GREEN_7     (RGB_Color_TypeDef){0,240,0} //绿色7
#define GREEN_6     (RGB_Color_TypeDef){0,200,0} //绿色6
#define GREEN_5     (RGB_Color_TypeDef){0,100,0} //绿色5
#define GREEN_4     (RGB_Color_TypeDef){0,60,0} //绿色4
#define GREEN_3     (RGB_Color_TypeDef){0,30,0} //绿色3
#define GREEN_2     (RGB_Color_TypeDef){0,20,0}  //绿色2
#define GREEN_1     (RGB_Color_TypeDef){0,10,0}  //绿色1

#define GREEN_TO_SKY_1     (RGB_Color_TypeDef){0,255,70} 
#define GREEN_TO_SKY_2     (RGB_Color_TypeDef){0,255,140} 
#define GREEN_TO_SKY_3     (RGB_Color_TypeDef){0,255,200} 

#define SKY      (RGB_Color_TypeDef){0,255,255}  //天蓝色
#define SKY_7     (RGB_Color_TypeDef){0,240,240} //天蓝色7
#define SKY_6     (RGB_Color_TypeDef){0,200,200} //天蓝色6
#define SKY_5     (RGB_Color_TypeDef){0,100,100} //天蓝色5
#define SKY_4     (RGB_Color_TypeDef){0,60,60} //天蓝色4
#define SKY_3     (RGB_Color_TypeDef){0,30,30} //天蓝色3
#define SKY_2     (RGB_Color_TypeDef){0,20,20} 	 //天蓝色2
#define SKY_1     (RGB_Color_TypeDef){0,10,10} 	 //天蓝色1

#define SKY_TO_BLUE_1      (RGB_Color_TypeDef){0,200,255}  
#define SKY_TO_BLUE_2      (RGB_Color_TypeDef){0,140,255}  
#define SKY_TO_BLUE_3      (RGB_Color_TypeDef){0,70,255}  

#define BLUE     		(RGB_Color_TypeDef){0,0,255}   //蓝色
#define BLUE_7     (RGB_Color_TypeDef){0,0,240} //蓝色7
#define BLUE_6     (RGB_Color_TypeDef){0,0,200} //蓝色6
#define BLUE_5     (RGB_Color_TypeDef){0,0,100} //蓝色5
#define BLUE_4     (RGB_Color_TypeDef){0,0,60} //蓝色4
#define BLUE_3     (RGB_Color_TypeDef){0,0,30} //蓝色3
#define BLUE_2     (RGB_Color_TypeDef){0,0,20} 	//蓝色2
#define BLUE_1     (RGB_Color_TypeDef){0,0,10} 	//蓝色1

#define BLUE_TO_MAGENTA_1     		(RGB_Color_TypeDef){70,0,255} 
#define BLUE_TO_MAGENTA_2     		(RGB_Color_TypeDef){140,0,255} 
#define BLUE_TO_MAGENTA_3     		(RGB_Color_TypeDef){200,0,255} 

#define MAGENTA 		  (RGB_Color_TypeDef){255,0,255}  //粉色
#define MAGENTA_7     (RGB_Color_TypeDef){240,0,240} //粉色7
#define MAGENTA_6     (RGB_Color_TypeDef){200,0,200} //粉色6
#define MAGENTA_5     (RGB_Color_TypeDef){100,0,100} //粉色5
#define MAGENTA_4     (RGB_Color_TypeDef){60,0,60} //粉色4
#define MAGENTA_3     (RGB_Color_TypeDef){30,0,30} //粉色3
#define MAGENTA_2     (RGB_Color_TypeDef){20,0,20} 	 //粉色2
#define MAGENTA_1     (RGB_Color_TypeDef){10,0,10} 	 //粉色1

#define MAGENTA_TO_RED_1 		  (RGB_Color_TypeDef){255,0,200}
#define MAGENTA_TO_RED_2 		  (RGB_Color_TypeDef){255,0,140}
#define MAGENTA_TO_RED_3 		  (RGB_Color_TypeDef){255,0,70}


//extern RGB_Color_TypeDef Argb_led_strip[];
//static void Reset_Load(void);
//void RGB_SendArray(void);
//void RGB_Flush(void);
void RGB_SetOne_Color(uint16_t LedId,RGB_Color_TypeDef Color);
void RGB_SetMore_Color(uint16_t head, uint16_t heal,RGB_Color_TypeDef color);
void RGB_Set_Color_More(uint16_t head, uint16_t heal,RGB_Color_TypeDef* color);
void RGB_Show_6(void);
void Argb_init(void);
void Set_CCR(uint8_t ccr);

//void Map_array_to_lights(RGB_Color_TypeDef (*color)[8]);
#endif
