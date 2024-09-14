#include "at32f402_405.h"               // Device header
#include "at32_encoder.h" 
#include "at32_oled.h"
#include "at32_mykeyboard.h"
//void delay(uint32_t time);
//uint32_t counter;

int8_t Encoder_num;

//A相：PB4			B相：PB3
void Encoder_Init(void)
{
  gpio_init_type gpio_init_struct;
	exint_init_type exint_init_struct;
	
  /* enable the button clock */
  crm_periph_clock_enable(ENCODER_GPIO_CRM_CLK, TRUE);
  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* configure button pin as input with pull-up/pull-down */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = ENCODER_A_PIN | ENCODER_B_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
  gpio_init(ENCODER_GPIO, &gpio_init_struct);
	
	 scfg_exint_line_config(ENCODER_A_SCFG_PORT_GPIO, ENCODER_A_SCFG_SOURCE);
	 scfg_exint_line_config(ENCODER_B_SCFG_PORT_GPIO, ENCODER_B_SCFG_SOURCE);
	
  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = ENCODER_A_EXINT_LINE |ENCODER_B_EXINT_LINE;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
	
  nvic_irq_enable(EXINT4_IRQn, 1, 0);
	nvic_irq_enable(EXINT3_IRQn, 1, 1);
	
	
	
}

void EXINT4_IRQHandler(void)//A相中断
{
  if(exint_interrupt_flag_get(EXINT_LINE_4) != RESET)
  {			
		if (gpio_input_data_bit_read(ENCODER_A_GPIO,ENCODER_A_PIN)==SET)
		{
			if (gpio_input_data_bit_read(ENCODER_B_GPIO,ENCODER_B_PIN)==SET)
			{
				Encoder_num--;
				Encoder_Volume_DEC();
			}
		}
    exint_flag_clear(EXINT_LINE_4);
  }
}

void EXINT3_IRQHandler(void)//B相中断
{
  if(exint_interrupt_flag_get(EXINT_LINE_3) != RESET)
  {
		if (gpio_input_data_bit_read(ENCODER_B_GPIO,ENCODER_B_PIN)==SET)
		{
			if (gpio_input_data_bit_read(ENCODER_A_GPIO,ENCODER_A_PIN)==SET)
			{
				Encoder_num++;
				Encoder_Volume_INC();
			}
		}
    exint_flag_clear(EXINT_LINE_3);
  }
}

int8_t Encoder_value_get(void)
{
		return Encoder_num;
}

void Show_Encoder_value(void)
{
	OLED_ShowString(4,1,"Volume:");
	OLED_ShowSignedNum(4,8,Encoder_value_get()%100,3);

}
