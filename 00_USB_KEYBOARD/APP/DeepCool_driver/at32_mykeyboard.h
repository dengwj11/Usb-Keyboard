#ifndef __AT32_MYKEYBOARD__
#define __AT32_MYKEYBOARD__

#include "at32f402_405.h"               // Device header
#include "at32_usb.h"
//	Row0~5:PA1~PA6
//	Col0~4:PA7,PB0,PB1,PB2,PA11
/*********************引脚配置begin*********************************/
#define ROW_GPIO                        GPIOA
#define ROW0_PIN                         GPIO_PINS_1
#define ROW0_GPIO                        GPIOA
#define ROW0_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PC0

#define ROW1_PIN                         GPIO_PINS_2
#define ROW1_GPIO                        GPIOA
#define ROW1_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PC1

#define ROW2_PIN                         GPIO_PINS_3
#define ROW2_GPIO                        GPIOA
#define ROW2_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PC2

#define ROW3_PIN                         GPIO_PINS_4
#define ROW3_GPIO                        GPIOA
#define ROW3_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PC3

#define ROW4_PIN                         GPIO_PINS_5
#define ROW4_GPIO                        GPIOA
#define ROW4_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PC4

#define ROW5_PIN                         GPIO_PINS_6
#define ROW5_GPIO                        GPIOA
#define ROW5_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PC5
//	Col0~4:PA7,PB0,PB1,PB2,PA11
//#define COL_GPIO                        GPIOA
#define COL0_PIN                         GPIO_PINS_7
#define COL0_GPIO                        GPIOA
#define COL0_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PA2

#define COL1_PIN                         GPIO_PINS_0
#define COL1_GPIO                        GPIOB
#define COL1_GPIO_CRM_CLK                CRM_GPIOB_PERIPH_CLOCK//PA3

#define COL2_PIN                         GPIO_PINS_1
#define COL2_GPIO                        GPIOB
#define COL2_GPIO_CRM_CLK                CRM_GPIOB_PERIPH_CLOCK//PA4

#define COL3_PIN                         GPIO_PINS_2
#define COL3_GPIO                        GPIOB
#define COL3_GPIO_CRM_CLK                CRM_GPIOB_PERIPH_CLOCK//PA5

#define COL4_PIN                         GPIO_PINS_11
#define COL4_GPIO                        GPIOA
#define COL4_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK//PA10
/*********************引脚配置end*********************************/

/*********************引脚设置与读取begin*********************************/
#define SET_ROW0                        ROW0_GPIO->scr = ROW0_PIN;
#define SET_ROW1                        ROW1_GPIO->scr = ROW1_PIN;
#define SET_ROW2                        ROW2_GPIO->scr = ROW2_PIN;
#define SET_ROW3                        ROW3_GPIO->scr = ROW3_PIN;
#define SET_ROW4                        ROW4_GPIO->scr = ROW4_PIN;
#define SET_ROW5                        ROW5_GPIO->scr = ROW5_PIN;
																					 
#define RESET_ROW0                      ROW0_GPIO->clr = ROW0_PIN;
#define RESET_ROW1                      ROW1_GPIO->clr = ROW1_PIN;
#define RESET_ROW2                      ROW2_GPIO->clr = ROW2_PIN;
#define RESET_ROW3                      ROW3_GPIO->clr = ROW3_PIN;
#define RESET_ROW4                      ROW4_GPIO->clr = ROW4_PIN;
#define RESET_ROW5                      ROW5_GPIO->clr = ROW5_PIN;

#define READ_COL0                       gpio_input_data_bit_read(COL0_GPIO,COL0_PIN)
#define READ_COL1                       gpio_input_data_bit_read(COL1_GPIO,COL1_PIN)
#define READ_COL2                       gpio_input_data_bit_read(COL2_GPIO,COL2_PIN)
#define READ_COL3                       gpio_input_data_bit_read(COL3_GPIO,COL3_PIN)
#define READ_COL4                       gpio_input_data_bit_read(COL4_GPIO,COL4_PIN)
/*********************引脚设置与读取end*********************************/
#define KEYB_LAYER_NUM      						2  


/*********************数据发送区位下标与数据对应关系begin******************/
#define LEFTCTRL							0 
#define LEFTSHIFT							1 
#define LEFTALT							  2
#define LEFTGUI							  3			//win
#define RIGHTCTRL							4
#define RIGHTSHIFT						5 
#define RIGHTALT							6
#define RIGHTGUI							7    // Byte0

																		//Byte1常
#define KEYBOARD_A							16 																	
#define KEYBOARD_B							17 
#define KEYBOARD_C							18 
#define KEYBOARD_D							19 
#define KEYBOARD_E							20
#define KEYBOARD_F							21 
#define KEYBOARD_G							22 
#define KEYBOARD_H							23 // Byte2
 
#define KEYBOARD_I							24 
#define KEYBOARD_J							25 
#define KEYBOARD_K							26 
#define KEYBOARD_L							27 
#define KEYBOARD_M							28 
#define KEYBOARD_N							29 
#define KEYBOARD_O							30
#define KEYBOARD_P							31 // Byte3

#define KEYBOARD_Q							32    
#define KEYBOARD_R							33 
#define KEYBOARD_S							34 
#define KEYBOARD_T							35 
#define KEYBOARD_U							36 
#define KEYBOARD_V							37 
#define KEYBOARD_W							38 
#define KEYBOARD_X							39 // Byte4

#define KEYBOARD_Y							40   
#define KEYBOARD_Z							41 
#define WOW_1								42 //! and1
#define AT_2								43 //@ and2
#define JING_3							44 //# and3
#define YUAN_4							45 //$ and4
#define BAIFEN_5						46 //% and5
#define PINGFANG_6					47 //^ and6

#define YU_7							  48 //& and7   
#define XING_8							49 //* and8
#define ZUOKUO_9					  50 //( and9
#define YOUKUO_0						51 //) and0
#define RETURN_ENTER					52 
#define ESCAPE							  53  //Esc
#define BACKSPACE							54 //左删
#define TAB										55 // Byte6

#define SPACEBAR							56 //空格 
#define UNDERSCORE__					57 //- and_
#define DENGYU_JIA						58 //=and+
#define FANGKUO_L							59 //[and{
#define FANGKUO_R							60 //]and}
#define FAN_XIEGANG						61 //\and|
#define NON_US							  62 //???
#define FENG_MAO							63 //;and: // Byte7

#define YINGHAO							  64 //'and"
#define GRAVEACCENT_TILDE		  65 //`and~
#define DOUHAO								66 	//,and<	 
#define JUHAO									67 //.and>
#define XIEGANG								68 // /and?
#define CAPSLOCK							69 //CapsLOCK
#define F_1							70//F1
#define F_2							71 // F2 Byte8

#define F_3							72 //F3
#define F_4							73 //F4
#define F_5							74 //F5
#define F_6							75 //F6
#define F_7							76 //F7
#define F_8							77 //F8
#define F_9							78 //F9
#define F_10						79 //F10 Byte9

#define F_11						80//F11
#define F_12						81//F12
#define PRINTSCREEN						82 
#define SCR_LOCK							83 
#define PAUSE									84 
#define INSERT								85 
#define HOME									86 		 
#define PAGE_UP								87 // Byte10

#define DELETE_FORWARD				88 
#define END										89 
#define PAGE_DOWN							90
#define RIGHT_ARROW						91 
#define LEFT_ARROW						92 
#define DOWN_ARROW						93 
#define UP_ARROW							94 
#define NUM_LOCK							95 // Num Byte11

#define XIEGANG_NUM						96 	//数字键盘上 /
#define XING_NUM							97 //数字键盘上 *
#define JIAN_NUM							98  //数字键盘上 -
#define JIA_NUM								99  //数字键盘上 +
#define ENTER_NUM							100//数字键盘上 ENTER
#define END_1									101 //1 AND END
#define DOWNARROW_2						102 //2 AND DOWN
#define PAGEDN_3							103 //3 AND PGDN  Byte12

#define LEFTARROW_4						104 //4 AND ...
#define NUM_5									105 //5 AND ...
#define RIGHTARROW_6					106 //6 AND ...		 
#define HOME_7								107 //7 AND ...
#define UPARROW_8							108 //8 AND ...
#define PAGEUP_9							109 //9 AND ...
#define INSERT_0							110 //0 AND ...
#define DELETE_DIAN						111 //. AND ...    Byte13

#define NON_US_XIE						112 //Non-US\and|
#define APPLICATION						113 //鼠标右键			Byte14(112~119)


#define PLAY_PAUSE						120  //		开始暂停
#define NXET_TRACK						121  //		下一曲
#define PREVIOUS_TRACK				122  //		上一曲
#define MUTE						    	123  //		静音
#define VOLUME_INC						124  //		音量+
#define VOLUME_DEC						125  //		音量-
#define FAST_FORWARD				  126  //		快进
#define REWIND						    127  //		倒带  Byte15(120~127)

#define NULL_KB						    128  //		无操作

/*********************数据发送区位下标与数据对应关系end********************/



void Keyboard_gpio_init(void);
void Keyboard_init(void);

void keyboard_scanning(void);
//void keyboard_send(uint8_t *string, uint8_t len,uint8_t shortcut_flag);
void keyboard_send(uint8_t *string,uint8_t DUOMEITI);

void keyboard_map_change(uint8_t row,uint8_t col,uint8_t key_num);
//void change_test(void);

void Encoder_Volume_INC(void);
void Encoder_Volume_DEC(void);
void Encoder_PLAY_PAUSE(void);
#endif

