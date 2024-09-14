#ifndef __AT32_KEYBOARD__
#define __AT32_KEYBOARD__

#include "at32f402_405.h"               // Device header
//	Row0~5:PC0~PC5
//	Col0~4:PA1~PA5
/*********************引脚配置begin*********************************/
#define ROW_GPIO                        GPIOC
#define ROW0_PIN                         GPIO_PINS_0
#define ROW0_GPIO                        GPIOC
#define ROW0_GPIO_CRM_CLK                CRM_GPIOC_PERIPH_CLOCK//PC0

#define ROW1_PIN                         GPIO_PINS_1
#define ROW1_GPIO                        GPIOC
#define ROW1_GPIO_CRM_CLK                CRM_GPIOC_PERIPH_CLOCK//PC1

#define ROW2_PIN                         GPIO_PINS_2
#define ROW2_GPIO                        GPIOC
#define ROW2_GPIO_CRM_CLK                CRM_GPIOC_PERIPH_CLOCK//PC2

#define ROW3_PIN                         GPIO_PINS_3
#define ROW3_GPIO                        GPIOC
#define ROW3_GPIO_CRM_CLK                CRM_GPIOC_PERIPH_CLOCK//PC3

#define ROW4_PIN                         GPIO_PINS_4
#define ROW4_GPIO                        GPIOC
#define ROW4_GPIO_CRM_CLK                CRM_GPIOC_PERIPH_CLOCK//PC4

#define ROW5_PIN                         GPIO_PINS_5
#define ROW5_GPIO                        GPIOC
#define ROW5_GPIO_CRM_CLK                CRM_GPIOC_PERIPH_CLOCK//PC5
//	Col0~4:PA1~PA5
#define COL_GPIO                        GPIOA
#define COL0_PIN                         GPIO_PINS_1
#define COL0_GPIO                        GPIOA
#define COL0_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PA1

#define COL1_PIN                         GPIO_PINS_2
#define COL1_GPIO                        GPIOA
#define COL1_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PA2

#define COL2_PIN                         GPIO_PINS_3
#define COL2_GPIO                        GPIOA
#define COL2_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PA3

#define COL3_PIN                         GPIO_PINS_4
#define COL3_GPIO                        GPIOA
#define COL3_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PA4

#define COL4_PIN                         GPIO_PINS_5
#define COL4_GPIO                        GPIOA
#define COL4_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PA5
/*********************引脚配置end*********************************/

/*********************引脚设置与读取begin*********************************/
#define SET_ROW0                        ROW_GPIO->scr = ROW0_PIN;
#define SET_ROW1                        ROW_GPIO->scr = ROW1_PIN;
#define SET_ROW2                        ROW_GPIO->scr = ROW2_PIN;
#define SET_ROW3                        ROW_GPIO->scr = ROW3_PIN;
#define SET_ROW4                        ROW_GPIO->scr = ROW4_PIN;
#define SET_ROW5                        ROW_GPIO->scr = ROW5_PIN;

#define RESET_ROW0                      ROW_GPIO->clr = ROW0_PIN;
#define RESET_ROW1                      ROW_GPIO->clr = ROW1_PIN;
#define RESET_ROW2                      ROW_GPIO->clr = ROW2_PIN;
#define RESET_ROW3                      ROW_GPIO->clr = ROW3_PIN;
#define RESET_ROW4                      ROW_GPIO->clr = ROW4_PIN;
#define RESET_ROW5                      ROW_GPIO->clr = ROW5_PIN;

#define READ_COL0                       gpio_input_data_bit_read(COL_GPIO,COL0_PIN);
#define READ_COL1                       gpio_input_data_bit_read(COL_GPIO,COL1_PIN);
#define READ_COL2                       gpio_input_data_bit_read(COL_GPIO,COL2_PIN);
#define READ_COL3                       gpio_input_data_bit_read(COL_GPIO,COL3_PIN);
#define READ_COL4                       gpio_input_data_bit_read(COL_GPIO,COL4_PIN);
/*********************引脚设置与读取end*********************************/

/*********************数据区begin*********************************/
uint8_t key_state_temp[6]={0};//按键当前状态，每一行数据状态分别放在一个字节中的低5位
uint8_t key_state_old[6]={0};//上一次扫描的状态
/*********************数据区begin*********************************/

void Keyboard_gpio_init(void);
void Keyboard_init(void);
#endif