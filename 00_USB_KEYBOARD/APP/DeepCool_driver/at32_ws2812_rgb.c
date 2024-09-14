#include "at32_ws2812_rgb.h"
//#include "stdlib.h"
#include "stdio.h"
#include "delay.h"
#include "at32_oled.h"
//#include <string.h>
//#include "at32_timer.h"

uint8_t timer_period = 0;
uint8_t rgb_channel_pulse = 0;

#define RGB_BIT0   34		//15	 ====arr(=10*ϵͳƵ��/16-1)*1/4
#define RGB_BIT1   100	//45   ====arr(=10*ϵͳƵ��/16-1)*3/4
//RGB_Color_TypeDef Argb_led_strip[Pixel_NUM] = 
//{
//	WHITE_4,WHITE_4,WHITE_4,WHITE_4,WHITE_4,WHITE_4,
//	WHITE_4,WHITE_4,WHITE_4,WHITE_4,WHITE_4,WHITE_4,
//	WHITE_4,WHITE_4,WHITE_4,WHITE_4,WHITE_4,WHITE_4,
//	WHITE_4,WHITE_4,WHITE_4,WHITE_4,WHITE_4,WHITE_4,
//};


/*��ά����������PWM������飬ÿһ��24�����ݴ���һ��LED�����һ��24��0���ڸ�λ*/
static unsigned char Pixel_Buf[Pixel_NUM+10][24]={0};

//static unsigned char Buf[24]={
//RGB_BIT0,RGB_BIT0,RGB_BIT0,RGB_BIT0,RGB_BIT0,RGB_BIT0,
//RGB_BIT0,RGB_BIT0,RGB_BIT0,RGB_BIT0,RGB_BIT0,RGB_BIT0,
//RGB_BIT0,RGB_BIT0,RGB_BIT0,RGB_BIT0,RGB_BIT0,RGB_BIT0,

//};
////unsigned short PP_Buf[(Pixel_NUM)*24];
//unsigned short g_ws2812_0;
//unsigned short g_ws2812_1;



/*�����240λ���RESET��ֹ�ź�*/
/*���һ��װ��9*24��0�����216������ռ�ձ�Ϊ0��PWM������Ϊ���reset��ʱ  >250us*/
//static void Reset_Load(void)
//{
//		for(uint16_t i=0;i<Pixel_NUM;i++)
//	{}
//		delay_us(250);
//}



/*
���ܣ���������Pixel_Buf[Pixel_NUM+1][24]�ڵ����ݣ����͵����ݱ��洢����ʱ��1ͨ��1��CCR�Ĵ��������ڿ���PWMռ�ձ�
������(&htim1)��ʱ��1��(TIM_CHANNEL_1)ͨ��1��((uint32_t *)Pixel_Buf)���������飬
			(Pixel_NUM+1)*24)���͸����������������
*/
//void RGB_SendArray(void)
//{
////	unsigned int i;
////		for(i=0;i<(Pixel_NUM)*24;i++)
////	{
////		PP_Buf[i]=Pixel_Buf[i/24][i%24];
////	}
////		PP_Buf[0]=0;
//		RGB_DMA_CHANNEL->maddr = (uint32_t)Pixel_Buf;//Pixel_Buf  PP_Buf
//    RGB_DMA_CHANNEL->dtcnt_bit.cnt = (Pixel_NUM)*24;
////	dmax_channely->dtcnt_bit.cnt = dma_init_struct->buffer_size;
////	tmr_output_enable(RGB_TMR, TRUE);
//	dma_channel_enable(RGB_DMA_CHANNEL, TRUE);
////	
////	 while(dma_flag_get(DMA1_FDT5_FLAG) == RESET);	
////		tmr_output_enable(RGB_TMR, FALSE);
////	dma_channel_enable(RGB_DMA_CHANNEL, FALSE);
////	dma_flag_clear(DMA1_FDT5_FLAG);
////	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)Pixel_Buf,(Pixel_NUM+1)*24);
//}




/*
���ܣ��趨����RGB LED����ɫ���ѽṹ����RGB��24BITת��Ϊ0���1��
������LedIdΪLED��ţ�Color���������ɫ�ṹ��
*/
void RGB_SetOne_Color(uint16_t LedId,RGB_Color_TypeDef Color)
{
	uint8_t i; 
//	g_ws2812_0 = (unsigned short)(((uint32_t) 3 * (timer_period + 1)) / 12);
//	g_ws2812_1 = (unsigned short)(((uint32_t) 9 * (timer_period + 1)) / 12);
	if(LedId > Pixel_NUM)return; //avoid overflow ��ֹд��ID����LED����
	//�����Ƕ� Pixel_Buf[LedId][i]д��һ�������ڸߵ�ƽ�ĳ���ʱ�䣨����˵ʱPWM��ռ�ձȼĴ���CCR1����
//	for(i=0;i<8;i++) Pixel_Buf[LedId][i]   = ( ((Color.R8) & (1 << (7 -i)))? (RGB_BIT1):RGB_BIT0 );//����ĳһ��0~7ת�����G
//	for(i=8;i<16;i++) Pixel_Buf[LedId][i]  = ( ((Color.G8) & (1 << (15-i)))? (RGB_BIT1):RGB_BIT0 );//����ĳһ��8~15ת�����R
//	for(i=16;i<24;i++) Pixel_Buf[LedId][i] = ( ((Color.B8) & (1 << (23-i)))? (RGB_BIT1):RGB_BIT0 );//����ĳһ��16~23ת�����B

	for(i=0;i<8;i++) Pixel_Buf[LedId][i]   = ( ((Color.G8) & (1 << (7 -i)))? (RGB_BIT1):RGB_BIT0 );//����ĳһ��0~7ת�����G
	for(i=8;i<16;i++) Pixel_Buf[LedId][i]  = ( ((Color.R8) & (1 << (15-i)))? (RGB_BIT1):RGB_BIT0 );//����ĳһ��8~15ת�����R
	for(i=16;i<24;i++) Pixel_Buf[LedId][i] = ( ((Color.B8) & (1 << (23-i)))? (RGB_BIT1):RGB_BIT0 );//����ĳһ��16~23ת�����B	
}

////����RGB_SetOne_Color��������ɶԶ��LED����ɫ���á�
void RGB_SetMore_Color(uint16_t head, uint16_t heal,RGB_Color_TypeDef color)
{
    uint16_t i=0;
    for(i=head;i<=heal;i++)
    {
        RGB_SetOne_Color(i,color) ;
    }
}

////����RGB_SetOne_Color��������ɶԶ��LED����ɫ���á�
void RGB_Set_Color_More(uint16_t head, uint16_t heal,RGB_Color_TypeDef* color)
{
    uint16_t i=0,j=0;
    for(i=head,j=0;i<=heal;i++)
    {
        RGB_SetOne_Color(i,color[j]) ;
				j++;
    }
}

// //ˢ��WS2812B�ư���ʾ����
//void RGB_Flush(void)
//{
//		RGB_Set_Color_More(0,Pixel_NUM-1,Argb_led_strip);
////		Reset_Load();
//    RGB_SendArray();  //��������
//     
//}



//�ƹ�ʵ�ֺ���
void RGB_Show_6(void)   
{

//      RGB_SetMore_Color(0,29,BLACK);  //������е�LED���� 
    RGB_SetMore_Color(0,50,SKY); 
//	Reset_Load();
//	for(uint16_t i=0;i<Pixel_NUM;i++)
//			Argb_led_strip[i]=(RGB_Color_TypeDef){6+i,10,0};
//			Argb_led_strip[1]=YELLOW;
//    RGB_SetMore_Color(0,7,color1);	
//	RGB_Flush(); 
	
}
///**
//  * @brief  ��32*8��ARGB����ӳ��ɵƴ����ݣ�0~255��
//  * @param  color  2ά����ָ�루n��8�У�
//  * @retval none
//  */
//void Map_array_to_lights(RGB_Color_TypeDef (*color)[8])
//{
//		uint8_t m=0,n=0;
//	  uint16_t start=0;
//		for(m=0;m<32;m++)//������
//		{
//			if(m%2==0)/*ż��*/  {start=(uint16_t)(255-8*m);    }
//			else			/*����*/	{start=(uint16_t)(255-8*(m-1));}
//			for(n=0;n<8;n++)
//			{
//				if(m%2==0)/*ż��*/  {RGB_SetOne_Color(start-n,color[m][n]);     }
//				else			/*����*/	{RGB_SetOne_Color(start-(15-n),color[m][n]);}
//			}	
//		}
//		
//}

void Argb_init(void)
{
	tmr_output_config_type tmr_output_struct;
  dma_init_type dma_init_struct = {0};
	crm_clocks_freq_type crm_clocks_freq_struct1 = {0};
	gpio_init_type  gpio_init_struct;
	crm_clocks_freq_get(&crm_clocks_freq_struct1);
	
	crm_periph_clock_enable(RGB_TMR_CLK, TRUE);
	crm_periph_clock_enable(RGB_DMA_CLK, TRUE);
	crm_periph_clock_enable(RGB_GPIO_CRM_CLK, TRUE);
	
	gpio_default_para_init(&gpio_init_struct);	
	gpio_init_struct.gpio_pins = RGB_PIN;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(RGB_GPIO, &gpio_init_struct);
	gpio_pin_mux_config(RGB_GPIO, RGB_PIN_SOURCE, RGB_PIN_MUX);
	
	timer_period = (crm_clocks_freq_struct1.sclk_freq / 800000 )/2 - 1;//sclk TMR2������apb1����  1.25us
	OLED_ShowNum(1,3,timer_period,5);
	rgb_channel_pulse = 0;//0  (uint16_t)(((uint32_t) 3 * (timer_period - 1)) / 12);
	
	tmr_base_init(RGB_TMR, timer_period, 1);
	tmr_cnt_dir_set(RGB_TMR, TMR_COUNT_UP);//���ϼ���
//	tmr_interrupt_enable(RGB_TMR, TMR_OVF_INT, TRUE);
//	nvic_irq_enable(TMR2_GLOBAL_IRQn, 0, 1);
//		tmr_counter_value_set(RGB_TMR,20);
	tmr_counter_enable(RGB_TMR, TRUE);
	

	tmr_output_default_para_init(&tmr_output_struct);
	tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_B;//���ģʽB,���ϼ��������޺���
	tmr_output_struct.oc_output_state = TRUE;
	tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;//��Ч��ƽΪdi
	tmr_output_struct.oc_idle_state = TRUE;
	tmr_output_struct.occ_output_state = TRUE;
	tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
	tmr_output_struct.occ_idle_state = FALSE;

	tmr_output_channel_config(RGB_TMR, RGB_TMR_CHANNEL, &tmr_output_struct);
	tmr_channel_value_set(RGB_TMR, RGB_TMR_CHANNEL, rgb_channel_pulse);
	tmr_output_enable(RGB_TMR, TRUE);
	
	tmr_dma_request_enable(RGB_TMR, TMR_OVERFLOW_DMA_REQUEST, TRUE);
	
	dma_reset(RGB_DMA_CHANNEL);
	dma_init_struct.buffer_size = (Pixel_NUM+10)*24;//
	dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_base_addr = (uint32_t)Pixel_Buf;
	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;//�ڴ�Ϊ�ֽڸ�ʽ
	dma_init_struct.memory_inc_enable = TRUE;
	dma_init_struct.peripheral_base_addr = (uint32_t)&RGB_TMR->c1dt;
	dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;//!!!��оƬ�ֲ�
	dma_init_struct.peripheral_inc_enable = FALSE;
	dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
	dma_init_struct.loop_mode_enable =TRUE;//ʹ��ѭ��ģʽFALSE
	dma_init(RGB_DMA_CHANNEL, &dma_init_struct);

	dma_channel_enable(RGB_DMA_CHANNEL, TRUE);
	dma_flexible_config(RGB_DMA, RGB_DMA_FLEX_CHANNEL, RGB_DMAMUX_OVERFLOW);
//		NVIC_EnableIRQ(DMA1_Channel7_4_IRQn);
//    dma_interrupt_enable(RGB_DMA_CHANNEL, DMA_FDT_INT, TRUE);
	
	

}

//void DMA1_Channel7_4_IRQHandler(void)
//{
//	   if(dma_flag_get(DMA1_FDT5_FLAG) != RESET)
//    {
//			dma_channel_enable(RGB_DMA_CHANNEL, FALSE);
//			tmr_channel_value_set(RGB_TMR, RGB_TMR_CHANNEL, 0);//?	
//			

//      dma_flag_clear(DMA1_FDT5_FLAG);
//    }
//}
/*�ȸߺ�ͣ��ߵ�ƽռ�ձ�*/
void Set_CCR(uint8_t ccr)/*0~60*/
{
	RGB_TMR->c1dt = ccr;

}



//void TMR2_GLOBAL_IRQHandler(void)
//{
//  if(tmr_flag_get(RGB_TMR, TMR_OVF_FLAG) != RESET)
//  {
//    /* add user code... */
//		tmr_counter_value_set(RGB_TMR,20);
//    tmr_flag_clear(RGB_TMR, TMR_OVF_FLAG);
//  }
//}
