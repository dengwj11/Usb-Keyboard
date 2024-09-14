#ifndef __AT32_SPI_H
#define __AT32_SPI_H

/* Includes ------------------------------------------------------------------*/
#include "at32f402_405.h"
//#include "delay.h"

/*SPI1Òý½Å¶¨Òå
* PB6---CS
* PB8---SCLK
* PB5---MOSI
* PB7---SPI_D/C
* PA12---OLED_RST
*/

#define SPI_CLK_PIN                       GPIO_PINS_8
#define SPI_CLK_GPIO                      GPIOB
//#define SPI_CLK_PINS_SOURCE 							GPIO_PINS_SOURCE8
//#define SPI_CLK_GPIO_MUX 									GPIO_MUX_0

#define SPI_MOSI_PIN                       GPIO_PINS_5
#define SPI_MOSI_GPIO                      GPIOB
//#define SPI_MOSI_PINS_SOURCE 							GPIO_PINS_SOURCE5
//#define SPI_MOSI_GPIO_MUX 									GPIO_MUX_0

#define SPI_CS_PIN                       GPIO_PINS_6
#define SPI_CS_GPIO                      GPIOB

#define SPI_DC_PIN                       GPIO_PINS_7
#define SPI_DC_GPIO                      GPIOB

#define SPI_RST_PIN                       GPIO_PINS_12
#define SPI_RST_GPIO                      GPIOA



//#define  BL  GPIOA->odt_bit.odt0 //PA0 
#define  DC  SPI_DC_GPIO->odt_bit.odt7 //PB7 DC
#define  CS  SPI_CS_GPIO->odt_bit.odt6 //PB6
#define  SDA SPI_MOSI_GPIO->odt_bit.odt5 //PB5
#define  RES SPI_RST_GPIO->odt_bit.odt12 //PA12
#define  SCL SPI_CLK_GPIO->odt_bit.odt8 //PB8
	
void spi_config(void);
void spi1_writ_byte(uint8_t dat);
void SpiInit(void);
	

#endif /* __SPI_H */
