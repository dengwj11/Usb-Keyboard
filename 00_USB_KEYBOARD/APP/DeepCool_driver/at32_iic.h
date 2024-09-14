#ifndef __AT32IIC_H
#define __AT32IIC_H

#include "i2c_application.h"
#include "at32_oled.h"
#define I2C_TIMEOUT                      0xFFFFFFF

//#define I2Cx_CLKCTRL                   0x2170FAFA   //10K
//#define I2Cx_CLKCTRL                   0x80E06565   //50K
#define I2Cx_CLKCTRL                     0x80E03030   //100K
//#define I2Cx_CLKCTRL                   0x20E0355F   //200K

#define I2Cx_ADDRESS                     0x78      //oled的从机地址

#define I2Cx_PORT                        I2C1
#define I2Cx_CLK                         CRM_I2C1_PERIPH_CLOCK
#define I2Cx_DMA                         DMA1
#define I2Cx_DMA_CLK                     CRM_DMA1_PERIPH_CLOCK

#define I2Cx_SCL_GPIO_CLK                CRM_GPIOC_PERIPH_CLOCK
#define I2Cx_SCL_GPIO_PIN                GPIO_PINS_6
#define I2Cx_SCL_GPIO_PinsSource         GPIO_PINS_SOURCE6
#define I2Cx_SCL_GPIO_PORT               GPIOC
#define I2Cx_SCL_GPIO_MUX                GPIO_MUX_4

#define I2Cx_SDA_GPIO_CLK                CRM_GPIOC_PERIPH_CLOCK
#define I2Cx_SDA_GPIO_PIN                GPIO_PINS_7
#define I2Cx_SDA_GPIO_PinsSource         GPIO_PINS_SOURCE7
#define I2Cx_SDA_GPIO_PORT               GPIOC
#define I2Cx_SDA_GPIO_MUX                GPIO_MUX_4

#define BUF_SIZE                         8

extern i2c_handle_type hi2cx;

void I2cInit(void);

//void TM1680InitTest(void);
//void TM1680Light(void);
//void TM1680Light2(void);

#endif

