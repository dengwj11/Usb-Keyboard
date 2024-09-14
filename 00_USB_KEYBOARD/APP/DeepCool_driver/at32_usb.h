#ifndef __AT32_USB__
#define __AT32_USB__

#include "at32f402_405.h"               // Device header
#include "delay.h"
#include "string.h"
#include "usb_conf.h"
#include "usb_core.h"
#include "usbd_int.h"
#include "keyboard_class.h"
#include "keyboard_desc.h"
#include "at32_mykeyboard.h"
#include "update.h" 

#define DEVICE_TYPE      	(0x04)
#define DEVICE_NUMBER    	(0x01)  //keyboard
#define FRAME_MIN_LEN		(0x06)
#define FRAME_HEAD    		(0x68)
#define FRAME_TAIL    		(0x16)

#define FUNCTION_CODE_CHANGE_KEY   		0x01
#define FUNCTION_CODE_GET_VERSION			0x02

typedef struct
{
	volatile unsigned char usb_rx_len;
	unsigned char usb_rx_buf[USBD_CUSTOM_IN_MAXPACKET_SIZE];
}USB_PACKET_STRUCT;


extern otg_core_type otg_core_struct;
extern uint8_t report_buf[USBD_CUSTOM_IN_MAXPACKET_SIZE];
//extern __IO uint8_t hid_suspend_flag;
//void keyboard_send_string(void *udev, uint8_t *string, uint8_t len);
//void keyboard_send_string(void *udev, uint8_t *string, uint8_t len,uint8_t shortcut_flag);
void keyboard_send_string(void *udev, uint8_t *string,uint8_t DUOMEITI);
void usb_clock48m_select(usb_clk48_s clk_s);
void usb_gpio_config(void);
void at32_usb_init(void);
void At32UsbSend(uint8_t* send_buffer, uint16_t len);
void At32UsbReceive(uint8_t* receive_buffer, unsigned char len);
void FramingData(unsigned char* str, unsigned short len);
//void OTG_IRQ_HANDLER(void);
void usb_delay_ms(uint32_t ms);
void usb_delay_us(uint32_t us);


#endif

