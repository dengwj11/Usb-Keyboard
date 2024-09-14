#ifndef __AT32_ENCODER__
#define __AT32_ENCODER__

#include "at32f402_405.h" 
 #include "at32_timer.h"
//3-5旋钮编码器控制
//输入捕获通道：TMR2_CH1-->PA0
//       TMR2_CH2 -->PA1

//A相：PB4			B相：PB3

#define ENCODER_GPIO                        GPIOB
#define ENCODER_GPIO_CRM_CLK                CRM_GPIOB_PERIPH_CLOCK

#define ENCODER_A_PIN                       GPIO_PINS_4//PB4
#define ENCODER_A_GPIO                      GPIOB
#define ENCODER_A_SCFG_PORT_GPIO  					SCFG_PORT_SOURCE_GPIOB
#define ENCODER_A_SCFG_SOURCE  							SCFG_PINS_SOURCE4
#define ENCODER_A_EXINT_LINE								EXINT_LINE_4
//#define ENCODER_A_PINS_SOURCE  							GPIO_PINS_SOURCE4
//#define ENCODER_A_GPIO_MUX 									GPIO_MUX_1

#define ENCODER_B_PIN                       GPIO_PINS_3//PB3
#define ENCODER_B_GPIO                      GPIOB
#define ENCODER_B_SCFG_PORT_GPIO  					SCFG_PORT_SOURCE_GPIOB
#define ENCODER_B_SCFG_SOURCE  							SCFG_PINS_SOURCE3
#define ENCODER_B_EXINT_LINE								EXINT_LINE_3
//#define ENCODER_B_PINS_SOURCE  							GPIO_PINS_SOURCE3
//#define ENCODER_B_GPIO_MUX 									GPIO_MUX_1






//#define ENCODER_TMR                      		TMR2
//#define ENCODER_TMR_CRM_CLK                  CRM_TMR2_PERIPH_CLOCK//

void Encoder_Init(void);
//int32_t Encoder_value_get(void);
int8_t Encoder_value_get(void);
void Show_Encoder_value(void);
#endif

