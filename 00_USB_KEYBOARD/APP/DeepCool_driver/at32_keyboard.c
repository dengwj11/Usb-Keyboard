#include "at32_keyboard.h"
#include "string.h"
/*6 rows  and 5 cols*/
/*矩阵键盘引脚定义
* PC0---row0				PA1---col0
* PC1---row1				PA2---col1
* PC2---row2				PA3---col2
* PC3---row3				PA4---col3
* PC4---row4				PA5---col4
* PC5---row5
*	Row0~5:PC0~PC5
*	Col0~4:PA1~PA5
*/


void Keyboard_gpio_init(void)
{
		/*行列GPIO初始化*/
	 gpio_init_type gpio_init_struct;
  /* enable the gpioa clock */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);
  /* configure the gpio */
	/*初始化ROW0~5,行扫描所以，行,输出*/
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;//输出
  gpio_init_struct.gpio_pins = ROW0_PIN| ROW1_PIN | ROW2_PIN| ROW3_PIN| ROW4_PIN| ROW5_PIN ;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;//上拉
  gpio_init(ROW_GPIO, &gpio_init_struct);
		/*初始化COL0~4,行扫描所以，列,输入*/
	 gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;//输入
	gpio_init_struct.gpio_pins = COL0_PIN| COL1_PIN | COL2_PIN| COL3_PIN| COL4_PIN ;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;//上拉
	gpio_init(COL_GPIO, &gpio_init_struct);
		/*赋初值，两端低电平代表按下*/
	gpio_bits_set(ROW_GPIO,ROW0_PIN| ROW1_PIN | ROW2_PIN| ROW3_PIN| ROW4_PIN| ROW5_PIN);
	gpio_bits_set(COL_GPIO,COL0_PIN| COL1_PIN | COL2_PIN| COL3_PIN| COL4_PIN);

}


void Keyboard_init(void)
{
	Keyboard_gpio_init();
	memset(key_state_temp,0,6);
	memset(key_state_old,0,6);//清空状态判断区
}



void keyboard_scanning(void)
{
	memcpy(key_state_old,key_state_temp,6);
}

