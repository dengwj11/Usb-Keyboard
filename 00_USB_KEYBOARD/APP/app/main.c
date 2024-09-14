/* 当前版本为2024.8.15 可自定义改键的版本 */

/* includes */
#include "at32f402_405.h"
#include "at32f402_405_clock.h"
#include "soft_timer.h"
#include "at32_timer.h"
#include "delay.h"
#include "at32_mykeyboard.h"
#include "at32_usb.h"
#include "at32_iic.h"
#include "at32_oled.h"
//#include "oledfont.h"
#include "at32_ws2812_rgb.h"
#include "at32_encoder.h" 
#include "at32_rgb_animation.h"

void Gpio_F4_init(void)
{
	gpio_init_type  gpio_init_struct;
	crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
	
	gpio_default_para_init(&gpio_init_struct);
	gpio_init_struct.gpio_pins = GPIO_PINS_4;
	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(GPIOF, &gpio_init_struct);
//		gpio_init_struct.gpio_pins = GPIO_PINS_5;
////	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
//		gpio_init(GPIOF, &gpio_init_struct);
 	GPIOF->scr = GPIO_PINS_4;//置1

}

int main(void)
{
	
	
	nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
	
  system_clock_config();
	
	delay_init();
	Timer1msInit();
	SoftTimerInit();
	Gpio_F4_init();
	OLED_Init();
	
	Encoder_Init();
	Argb_init();
	at32_usb_init();
  Keyboard_init();
	OLED_Clear();				//OLED清屏
   OLED_ShowString(1,1,"    DEEPCOOL    ");
	SoftTimerActiveEvent(Liushui, 0, 0);
	
//	SoftTimerActiveEvent(RGB_Mode, 0,20);
//	GPIOF->clr = GPIO_PINS_4;//置1
		SoftTimerActiveEvent(keyboard_scanning, LIUSHUI_ENDTIME+60, 20);//每隔20ms检测一次
		SoftTimerActiveEvent(Show_Encoder_value, LIUSHUI_ENDTIME+50, 50);
//		SoftTimerActiveEvent(light_breathing, LIUSHUI_ENDTIME+50, 20);
		
//		SoftTimerActiveEvent(keyboard_test, 0, 2000);
//	RGB_Show_6();
	while(1)
	{

//		Set_CCR(60);
//		delay_ms(1000);

//		Set_CCR(0);
//		delay_ms(1000);
		SoftTimerHandler();

	}
}

