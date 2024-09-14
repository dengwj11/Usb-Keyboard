#include "at32_spi.h"
#include "delay.h"
//#include "sa3860g.h"
/*SPI1引脚定义
* PB6---CS
* PB8---SCLK
* PB5---MOSI
* PB7---SPI_D/C
* PA12---OLED_RST
*/

/******************************************************************************
    函数说明：硬件SPIGPIO配置
		入口数据：无
		返回值：无
******************************************************************************/
static void spi_gpio_config(void)
{
	gpio_init_type gpio_initstructure;//
	
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);//
  gpio_default_para_init(&gpio_initstructure);//
	
	/* spi1 sck pin */
  gpio_initstructure.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;//
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;//
  gpio_initstructure.gpio_pull           = GPIO_PULL_NONE;  //
  gpio_initstructure.gpio_mode           = GPIO_MODE_MUX; //
  gpio_initstructure.gpio_pins = GPIO_PINS_5;//PA5
  gpio_init(GPIOA, &gpio_initstructure);//
  
  /* spi1 mosi pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_NONE;  
  gpio_initstructure.gpio_mode           = GPIO_MODE_MUX; 
  gpio_initstructure.gpio_pins = GPIO_PINS_7;//PA7 
  gpio_init(GPIOA, &gpio_initstructure);
  /*CS pin*/
  gpio_initstructure.gpio_pull           = GPIO_PULL_UP;//
  gpio_initstructure.gpio_mode           = GPIO_MODE_OUTPUT; //
  gpio_initstructure.gpio_pins = GPIO_PINS_4;//PA4
  gpio_init(GPIOA, &gpio_initstructure);
	
	/* 2024.7.15 configure the led gpio */
	gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_initstructure.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;//
	gpio_initstructure.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_initstructure.gpio_pins = GPIO_PINS_0|GPIO_PINS_2|GPIO_PINS_3;//PA1/PA2/PA3
	gpio_initstructure.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOA, &gpio_initstructure);	
	/* 2024.7.15  */
	GPIOA->odt_bit.odt4=1;
}

/**
  * @brief  硬件SPI的初始化配置.
  * @param  none
  * @retval none
  */
void spi_config(void)
{
	spi_gpio_config();//配置GPIO
	
  spi_init_type spi_init_struct;//
  crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE);//
	
  spi_default_para_init(&spi_init_struct);//
  spi_init_struct.transmission_mode = SPI_TRANSMIT_HALF_DUPLEX_TX;//
  spi_init_struct.master_slave_mode =SPI_MODE_MASTER;//
  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_64;//
  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;//
  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;//
  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_HIGH;//
  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
#if HW_SPI_EN == 0
	  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
#else
		spi_init_struct.cs_mode_selection =SPI_CS_HARDWARE_MODE;
#endif


  spi_init(SPI1, &spi_init_struct);
	
  spi_enable(SPI1, TRUE);//
	
	CS=1;
//	BL=0;
	
	RES = 1;
//	delay_ms(10);
//	RES = 0; 		//?′??￡?μíóDD§
//	delay_ms(50);
//	RES = 1;
//	delay_ms(100);
}

/**
  * @brief  硬件SPI的数据发送.
  * @param  uint8_t dat 要发送的字节 
  * @retval none
  */
void spi1_writ_byte(uint8_t dat)
{
    spi_i2s_data_transmit(SPI1,dat);
    while(spi_i2s_flag_get(SPI1, SPI_I2S_TDBE_FLAG) == RESET); //è?1?·???,?òò??±μè′y
    /*RSET±íê????D*/
    while(spi_i2s_flag_get(SPI1, SPI_I2S_BF_FLAG) == TRUE);//è?1??|?μ,?òò??±μè′y
}
/**
  * @brief  软件SPI的初始化配置.
  * @param  none
  * @retval none
  */
void SpiInit(void)
{
	gpio_init_type gpio_init_struct;

	/* enable the led clock */
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	
	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);

	/* configure the led gpio */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = SPI_CLK_PIN;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(SPI_CLK_GPIO, &gpio_init_struct);	
	
	gpio_init_struct.gpio_pins = SPI_MOSI_PIN;
	gpio_init(SPI_MOSI_GPIO, &gpio_init_struct);	
	
	gpio_init_struct.gpio_pins = SPI_CS_PIN;
	gpio_init(SPI_CS_GPIO, &gpio_init_struct);	
	
	gpio_init_struct.gpio_pins = SPI_DC_PIN;
	gpio_init(SPI_DC_GPIO, &gpio_init_struct);	
	
	gpio_init_struct.gpio_pins = SPI_RST_PIN;
	gpio_init(SPI_RST_GPIO, &gpio_init_struct);	
	
	DC=0;
	CS=0;
	SDA=0;
	RES=0;
	SCL=0;
//	BL=0;
//	
	//BL = 1;
	RES = 1;
	delay_ms(10);
	RES = 0; 		
	delay_ms(50);
	RES = 1;
	delay_ms(100);
}
