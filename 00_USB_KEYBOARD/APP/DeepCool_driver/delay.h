#ifndef __DELAY_H
#define __DELAY_H

#include "at32f402_405.h"
#include "stdio.h"
/* delay macros */
#define STEP_DELAY_MS                    50
/* delay variable */
static __IO uint32_t fac_us;
static __IO uint32_t fac_ms;

/* delay function */
void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void delay_sec(uint16_t sec);
//void usb_delay_ms(uint32_t ms);


#endif /* __DELAY_H */
