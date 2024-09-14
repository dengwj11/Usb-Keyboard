#include "at32_mykeyboard.h"
#include "string.h"
#include "myflash_store.h" 
#include "myflash.h" 
#include "at32_oled.h"
#include "at32_rgb_animation.h"
#include "soft_timer.h"
/*6 rows  and 5 cols*/
/*矩阵键盘引脚定义
* PA1---row0				PA7---col0
* PA2---row1				PB0---col1
* PA3---row2				PB1---col2
* PA4---row3				PB2---col3
* PA5---row4				PA11---col4
* PA6---row5
*	Row0~5:PA1~PA6
*	Col0~4:PA7,PB0,PB1,PB2,PA10
//	
*/

/*********************数据区begin*********************************/
 uint8_t key_state_temp[6]={0};//按键当前状态，每一行数据状态分别放在一个字节中的低5位
 uint8_t key_state_old[6]={0};//上一次扫描的状态，1代表按下，0代表未按下
	static uint8_t key_state_result[6]={0};//前后状态比较后，确认滤波结果，1代表按下了，0代表已松开
	static uint8_t key_state_result1[6]={0};
//static uint8_t key_ascii_code[6]={0};//存放由坐标映射的对应字符的asc码，最多6个
static uint8_t key_layer=0;

/*右边键盘的映射图:坐标--->ascii码（字母为左键盘的）*/
static unsigned char asc_iimap[KEYB_LAYER_NUM][6][5]=
{ 
		/*默认按键设置层*/
	 {{NULL_KB,		KEYBOARD_A,	KEYBOARD_C,		KEYBOARD_V,	 KEYBOARD_X},	//旋钮	 		a			c  	  v 	 	  x 
		{TAB,		 		NUM_LOCK,		XIEGANG_NUM,	XING_NUM,		 JIAN_NUM	 },	//Tab 			Num  	/ 	  *   	  -  
		{LEFTSHIFT,	HOME_7,			UPARROW_8,		PAGEUP_9,		 JIA_NUM 	 },	//shift  	  7  		8 	 	9  		  +
		{LEFTCTRL,	LEFTARROW_4,NUM_5,				RIGHTARROW_6,JIA_NUM 	 },	//ctrl  		4  		5 	 	6  		  +
		{LEFTALT,		END_1,			DOWNARROW_2,	PAGEDN_3,		 ENTER_NUM },	//Alt 		  1 		2 	 	3       Enter
//		{NULL_KB,		INSERT_0,		INSERT_0,			DELETE_DIAN, ENTER_NUM}}, //NUL(切换)	0  		0 	 	.       Enter
		{NULL_KB,		VOLUME_INC,		VOLUME_DEC,			FAST_FORWARD, REWIND}}, //NUL(切换)	音量+  	音量- 	快进    倒带

		/*用户自定义层1*/
	 {{NULL_KB,		WOW_1,			AT_2,					JING_3,			 YUAN_4		 },	//旋钮	 		1  	  2 	 	3  	 	  4
		{TAB,				F_10,				F_11,					F_12,				 KEYBOARD_A},	//Tab 			F10  	F11 	F12   	a  
		{RIGHTSHIFT,F_7,				F_8,					F_9,				 KEYBOARD_C},	//shift  	  F7  	F8 	 	F9  		c
		{RIGHTCTRL,	F_4,				F_5,					F_6,				 KEYBOARD_C},	//ctrl  		F4  	F5 	 	F6  		c
		{RIGHTALT,	F_1,				F_2,					F_3,				 KEYBOARD_V},	//Alt 		  F1 		F2 	 	F3      v
//		{NULL_KB,		SPACEBAR,		SPACEBAR,			BACKSPACE,	 KEYBOARD_V}},//NUL(切换)	空格  Backspace	    v
		{NULL_KB,		PLAY_PAUSE,		NXET_TRACK,			PREVIOUS_TRACK,	 MUTE}},//NUL(切换)	播放  下一曲	  上一曲  静音

};


/*********************数据区begin*********************************/

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
  gpio_init_struct.gpio_pins = ROW0_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;//上拉
  gpio_init(ROW0_GPIO, &gpio_init_struct);
	
	gpio_init_struct.gpio_pins = ROW1_PIN;
	gpio_init(ROW1_GPIO, &gpio_init_struct);
	
	gpio_init_struct.gpio_pins = ROW2_PIN;
	gpio_init(ROW2_GPIO, &gpio_init_struct);
	
	gpio_init_struct.gpio_pins = ROW3_PIN;
	gpio_init(ROW3_GPIO, &gpio_init_struct);
	
	gpio_init_struct.gpio_pins = ROW4_PIN;
	gpio_init(ROW4_GPIO, &gpio_init_struct);
	
	gpio_init_struct.gpio_pins = ROW5_PIN;
	gpio_init(ROW5_GPIO, &gpio_init_struct);
		
		/*初始化COL0~4,行扫描所以，列,输入*/
	 gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;//输入
	gpio_init_struct.gpio_pins = COL0_PIN ;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;//上拉
	gpio_init(COL0_GPIO, &gpio_init_struct);
	
	gpio_init_struct.gpio_pins = COL1_PIN ;
	gpio_init(COL1_GPIO, &gpio_init_struct);

	
	gpio_init_struct.gpio_pins = COL2_PIN ;
	gpio_init(COL2_GPIO, &gpio_init_struct);
	
	gpio_init_struct.gpio_pins = COL3_PIN ;
	gpio_init(COL3_GPIO, &gpio_init_struct);
	
	gpio_init_struct.gpio_pins = COL4_PIN ;
	gpio_init(COL4_GPIO, &gpio_init_struct);
		/*赋初值，两端低电平代表按下*/
	gpio_bits_set(ROW_GPIO,ROW0_PIN| ROW1_PIN | ROW2_PIN| ROW3_PIN| ROW4_PIN| ROW5_PIN);
	gpio_bits_set(GPIOA,COL0_PIN| COL4_PIN);
	gpio_bits_set(GPIOB,COL1_PIN| COL2_PIN| COL3_PIN);
}


static void Store_to_Kbmap(void)//Store_Data[1+60]--->map
{
		for(uint16_t i=0;i<(KEYB_LAYER_NUM)*30;i++)
	{
		asc_iimap[i/30][(i%30)/5][(i%30)%5]=Store_Data[i+1];
	}

}

static void Kbmap_to_Store(void)//map--->Store_Data[1+60]
{
//		for(i=0;i<(Pixel_NUM)*24;i++)
//	{
//		PP_Buf[i]=Pixel_Buf[i/24][i%24];
//	}

		for(uint16_t i=0;i<(KEYB_LAYER_NUM)*30;i++)
	{
		Store_Data[i+1]=asc_iimap[i/30][(i%30)/5][(i%30)%5];
	}
	
}

void Keyboard_init(void)
{
	uint8_t flash_first_flag=0;
	Keyboard_gpio_init();//引脚初始化
	memset(key_state_temp,0,6);
	memset(key_state_old,0,6);//清空状态判断区
	memset(key_state_result,0,6);
		memset(key_state_result1,0,6);
//	memset(key_ascii_code,0,6);
//	memset(board_buf,0,15);
	key_layer=1;//开始为默认层

	if(MyFlash_ReadByte(MYFLASH_SECTOR_ADDR) != 0xA5)
	{
     flash_first_flag=2;
	}
	Store_Init();//FLASH--->Store_Data[512]
	
	if(flash_first_flag==2)
	{
		Kbmap_to_Store();//map--->Store_Data[1+60]
	  Store_Save();////Store_Data[512]-->FLASH  //只有第一次启动Flash才需要这两行	
	}			
	Store_to_Kbmap();//Store_Data[1+60]--->map
}

uint8_t RGB_Mode_flag=1,oled_layer_add_flag;
uint16_t  KONGxian_flag,oled_layer_add;
uint8_t Volume_Flag;
unsigned char breathe_handle;
void keyboard_scanning(void)
{
	static uint8_t Power_flag=0,BoFang_flag=0;
	uint8_t Key_DUOMEITI=0x01,Usb_en=0;
	uint8_t board_buf[16]={0x00};
	uint16_t i=0,j=0/*,p_code=0*/;
	static uint8_t TRACK_flag=0;//0表示上次操作为上一首，1表示上次操作为下一首
	/*static uint8_t shortcuts_flag=0,last_fg=0,new_fg=0;*/
	uint16_t temp_0=0,Volume_Flag_temp=0;
//  static uint8_t longhit_flag0=0,longhit_flag1=0,longhit_flag2=0,longhit_flag3=0,longhit_flag4=0;//长按触发标志
	memcpy(key_state_old,key_state_temp,6);//读取之前将上一次状态装入
	memset(key_state_temp,0x00,6);//当前状态清空，防止污染下一次数据
	memset(board_buf,0x00,16);
	/*开始行扫描   OK*/
//		RESET_ROW0;
//			if(READ_COL0 == RESET) {key_state_temp[0]|=0x01;Map_Setone_color(0,0,WHITE);/*OLED_ShowNum(2,1,0,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(0,0,BLACK);}
//			if(READ_COL1 == RESET) {key_state_temp[0]|=0x02;Map_Setone_color(0,1,WHITE);/*OLED_ShowNum(2,1,1,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(0,1,BLACK);}  
//			if(READ_COL2 == RESET) {key_state_temp[0]|=0x04;Map_Setone_color(0,2,WHITE);/*OLED_ShowNum(2,1,2,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(0,2,BLACK);}  
//			if(READ_COL3 == RESET) {key_state_temp[0]|=0x08;Map_Setone_color(0,3,WHITE);/*OLED_ShowNum(2,1,3,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(0,3,BLACK);}  
//			if(READ_COL4 == RESET) {key_state_temp[0]|=0x10;Map_Setone_color(0,4,WHITE);/*OLED_ShowNum(2,1,4,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(0,4,BLACK);}  
//		SET_ROW0;
//	
//		RESET_ROW1;
//			if(READ_COL0 == RESET) {key_state_temp[1]|=0x01;Map_Setone_color(1,0,RED_TO_YELLOW_3);/*OLED_ShowNum(2,1,10,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(1,0,BLACK);} 
//			if(READ_COL1 == RESET) {key_state_temp[1]|=0x02;Map_Setone_color(1,1,RED_TO_YELLOW_3);/*OLED_ShowNum(2,1,11,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(1,1,BLACK);} 
//			if(READ_COL2 == RESET) {key_state_temp[1]|=0x04;Map_Setone_color(1,2,RED_TO_YELLOW_3);/*OLED_ShowNum(2,1,12,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(1,2,BLACK);} 
//			if(READ_COL3 == RESET) {key_state_temp[1]|=0x08;Map_Setone_color(1,3,RED_TO_YELLOW_3);/*OLED_ShowNum(2,1,13,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(1,3,BLACK);} 
//			if(READ_COL4 == RESET) {key_state_temp[1]|=0x10;Map_Setone_color(1,4,RED_TO_YELLOW_3);/*OLED_ShowNum(2,1,14,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(1,4,BLACK);} 
//		SET_ROW1;
//	
//		RESET_ROW2;
//			if(READ_COL0 == RESET) {key_state_temp[2]|=0x01;Map_Setone_color(2,0,YELLOW_TO_GREEN_3);/*OLED_ShowNum(2,1,20,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(2,0,BLACK);}  
//			if(READ_COL1 == RESET) {key_state_temp[2]|=0x02;Map_Setone_color(2,1,YELLOW_TO_GREEN_3);/*OLED_ShowNum(2,1,21,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(2,1,BLACK);}  
//			if(READ_COL2 == RESET) {key_state_temp[2]|=0x04;Map_Setone_color(2,2,YELLOW_TO_GREEN_3);/*OLED_ShowNum(2,1,22,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(2,2,BLACK);}  
//			if(READ_COL3 == RESET) {key_state_temp[2]|=0x08;Map_Setone_color(2,3,YELLOW_TO_GREEN_3);/*OLED_ShowNum(2,1,23,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(2,3,BLACK);}  
//			if(READ_COL4 == RESET) {key_state_temp[2]|=0x10;Map_Setone_color(2,4,YELLOW_TO_GREEN_3);/*OLED_ShowNum(2,1,24,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(2,4,BLACK);}  
//		SET_ROW2;
//		
//		RESET_ROW3;
//			if(READ_COL0 == RESET) {key_state_temp[3]|=0x01;Map_Setone_color(3,0,GREEN);/*OLED_ShowNum(2,1,30,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(3,0,BLACK);} 
//			if(READ_COL1 == RESET) {key_state_temp[3]|=0x02;Map_Setone_color(3,1,GREEN);/*OLED_ShowNum(2,1,31,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(3,1,BLACK);} 
//			if(READ_COL2 == RESET) {key_state_temp[3]|=0x04;Map_Setone_color(3,2,GREEN);/*OLED_ShowNum(2,1,32,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(3,2,BLACK);} 
//			if(READ_COL3 == RESET) {key_state_temp[3]|=0x08;Map_Setone_color(3,3,GREEN);/*OLED_ShowNum(2,1,33,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(3,3,BLACK);} 
//			if(READ_COL4 == RESET) {key_state_temp[3]|=0x10;Map_Setone_color(3,4,GREEN);/*OLED_ShowNum(2,1,34,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(3,4,BLACK);} 
//		SET_ROW3;                                                                                             

//		RESET_ROW4;
//			if(READ_COL0 == RESET) {key_state_temp[4]|=0x01;Map_Setone_color(4,0,SKY);/*OLED_ShowNum(2,1,40,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(4,0,BLACK);} 
//			if(READ_COL1 == RESET) {key_state_temp[4]|=0x02;Map_Setone_color(4,1,SKY);/*OLED_ShowNum(2,1,41,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(4,1,BLACK);} 
//			if(READ_COL2 == RESET) {key_state_temp[4]|=0x04;Map_Setone_color(4,2,SKY);/*OLED_ShowNum(2,1,42,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(4,2,BLACK);} 
//			if(READ_COL3 == RESET) {key_state_temp[4]|=0x08;Map_Setone_color(4,3,SKY);/*OLED_ShowNum(2,1,43,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(4,3,BLACK);} 
//			if(READ_COL4 == RESET) {key_state_temp[4]|=0x10;Map_Setone_color(4,4,SKY);/*OLED_ShowNum(2,1,44,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(4,4,BLACK);} 
//		SET_ROW4;
		
		RESET_ROW5;
			if(READ_COL0 == RESET) {key_state_temp[5]|=0x01;Map_Setone_color(5,0,MAGENTA);/*OLED_ShowNum(2,1,50,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(5,0,BLACK);}  
			if(READ_COL1 == RESET) {key_state_temp[5]|=0x02;Map_Setone_color(5,1,MAGENTA);/*OLED_ShowNum(2,1,51,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(5,1,BLACK);}  
			if(READ_COL2 == RESET) {key_state_temp[5]|=0x04;Map_Setone_color(5,2,MAGENTA);/*OLED_ShowNum(2,1,52,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(5,2,BLACK);}  
			if(READ_COL3 == RESET) {key_state_temp[5]|=0x08;Map_Setone_color(5,3,MAGENTA);/*OLED_ShowNum(2,1,53,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(5,3,BLACK);}  
			if(READ_COL4 == RESET) {key_state_temp[5]|=0x10;Map_Setone_color(5,4,MAGENTA);/*OLED_ShowNum(2,1,54,2);*/} else{if(RGB_Mode_flag==1) Map_Setone_color(5,4,BLACK);}  
		SET_ROW5;
	/*行扫描结束*/
			if(!(key_state_temp[0]|key_state_temp[1]|key_state_temp[2]|key_state_temp[3]|key_state_temp[4]|key_state_temp[5]))
			{
				if(RGB_Mode_flag==2)  {light_breathing();}
				KONGxian_flag++;
				if(KONGxian_flag>=960)  {KONGxian_flag=0;RGB_Mode_flag=2;}
			}
			else		{KONGxian_flag=0;RGB_Mode_flag=1;}


	for(i=0;i<6;i++)
	{
		key_state_result[i]=key_state_temp[i];	
		key_state_result1[i]=(key_state_old[i])&(~key_state_temp[i]);
	}
		
	//按键层切换
	if((key_state_old[5]&0x01)&&(!(key_state_temp[5]&0x01))) {key_layer++;key_layer %=KEYB_LAYER_NUM;}//松手检测,上升沿 层切换
	if(key_layer==0)	{OLED_ShowString(2,1,"Default layer   ");oled_layer_add_flag=1;}
	if(key_layer==1)	{OLED_ShowString(2,1,"Custom  layer   ");oled_layer_add_flag=1;}
	if(oled_layer_add_flag==1)	{oled_layer_add++;}
	if(oled_layer_add>=100)		{oled_layer_add_flag=0;oled_layer_add=0;OLED_ShowString(2,1,"                ");}
					/*将状态为按下的按键坐标映射为对应字符的asc码值   OK*/
	memset(key_state_old,0,6);
			for(i=0;i<6;i++)
			{
				for(j=0;j<5;j++)
				{
						temp_0=asc_iimap[key_layer][i][j];
						if((key_state_result[i]&(0x01<<j))&&(temp_0<120)) 
						{	
							board_buf[temp_0/8]|=(0x01<<(temp_0%8));
							
						}
						if((key_state_result1[i]&(0x01<<j))&&(temp_0==120||temp_0==123))
						{									
							board_buf[temp_0/8]|=(0x01<<(temp_0%8));	
							if(BoFang_flag<10)	BoFang_flag++;
							if(BoFang_flag==2) {Usb_en=1;}
						}//
						if((key_state_result1[i]&(0x01<<j))&&(temp_0==121))
						{		
								if(Power_flag==0)		{Power_flag=1;Usb_en=1;}
								if(TRACK_flag==2)		{Usb_en=1;}
								board_buf[temp_0/8]|=(0x01<<(temp_0%8));
								TRACK_flag=1;
						}//
						if((key_state_result1[i]&(0x01<<j))&&(temp_0==122))
						{			
							if(Power_flag==0)		{Power_flag=1;Usb_en=1;}
							if(TRACK_flag==1)		{Usb_en=1;}
							board_buf[temp_0/8]|=(0x01<<(temp_0%8));	
								TRACK_flag=2;
						}//
						if((key_state_result[i]&(0x01<<j))&&(temp_0>123)) 
						{	
							Volume_Flag_temp=1;
							if(Volume_Flag>3) {Volume_Flag=0;board_buf[temp_0/8]|=(0x01<<(temp_0%8));}//消抖
						
						}
				}
			}
			if(Volume_Flag_temp==1) {Volume_Flag_temp=0;Volume_Flag++;}
			if(board_buf[15]) {Key_DUOMEITI=0x02;}
			memset(key_state_result,0,6);//用完清零，防止污染下一次数据
			memset(key_state_result1,0,6);//用完清零，防止污染下一次数据

			keyboard_send(board_buf,Key_DUOMEITI);//映射
			if(Usb_en==1) {delay_us(600);keyboard_send(board_buf,Key_DUOMEITI);}//消除奇怪问题：按下（下/上一首），再按上/下一首会出现不断触发状态，只有再按一次结束

		
}



void keyboard_send(uint8_t *string,uint8_t DUOMEITI)
{
	     if(usbd_connect_state_get(&otg_core_struct.dev) == USB_CONN_STATE_CONFIGURED)
      {
        keyboard_send_string(&otg_core_struct.dev, string,DUOMEITI);
      }
}


void keyboard_map_change(uint8_t row,uint8_t col,uint8_t key_num)
{
	asc_iimap[1][row-1][col-1]=key_num;//改变map  
	Kbmap_to_Store();//map--->Store_Data[1+60]
	Store_Save();////Store_Data[512]-->FLASH
}

void Encoder_Volume_INC(void)
{
	uint8_t board_buf[16]={0x00};
	memset(board_buf,0x00,16);
	board_buf[VOLUME_INC/8]|=(0x01<<(VOLUME_INC%8));
	keyboard_send(board_buf,0x02);
}

void Encoder_Volume_DEC(void)
{
	uint8_t board_buf[16]={0x00};
	memset(board_buf,0x00,16);
	board_buf[VOLUME_DEC/8]|=(0x01<<(VOLUME_DEC%8));
	keyboard_send(board_buf,0x02);
}


void Encoder_PLAY_PAUSE(void)
{
	uint8_t board_buf[16]={0x00};
	memset(board_buf,0x00,16);
	board_buf[PLAY_PAUSE/8]|=(0x01<<(PLAY_PAUSE%8));
	keyboard_send(board_buf,0x02);
}


