/**
  **************************************************************************
  * @file     keyboard_class.h
  * @brief    usb hid keyboard header file
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

 /* define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEYBOARD_CLASS_H
#define __KEYBOARD_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_std.h"
#include "usbd_core.h"

/** @addtogroup AT32F402_405_middlewares_usbd_class
  * @{
  */

/** @addtogroup USB_keyboard_class
  * @{
  */

/** @defgroup USB_keyboard_class_endpoint_definition
  * @{
  */

/**
  * @brief usb hid use endpoint define
  */
#define USBD_KEYBOARD_IN_EPT                  0x81
#define USBD_CUSTOM_HID_IN_EPT                0x82
#define USBD_CUSTOM_HID_OUT_EPT               0x02
/**
  * @brief usb hid in and out max packet size define
  */
#define USBD_KEYBOARD_IN_MAXPACKET_SIZE       0x40
#define USBD_KEYBOARD_OUT_MAXPACKET_SIZE      0x40
#define USBD_CUSTOM_IN_MAXPACKET_SIZE           0x40
#define USBD_CUSTOM_OUT_MAXPACKET_SIZE          0x40
/**
  * @}
  */


typedef struct
{
  uint32_t hid_protocol;
  uint32_t hid_set_idle;
  uint32_t alt_setting;
  uint8_t hid_set_report[64];
//  uint8_t keyboard_buf[8];   //6键无冲
  uint8_t keyboard_buf[15+1];			//全键无冲
	//对于键盘：字节0为ID，字节1~15为标志位;对于多媒体控制：字节0为ID，字节1为标志位，其他保留
  __IO uint8_t hid_suspend_flag;
  __IO uint8_t g_u8tx_completed;
  uint8_t hid_state;
  uint8_t temp;

	uint8_t g_rxhid_buff[USBD_CUSTOM_OUT_MAXPACKET_SIZE];
  uint8_t g_txhid_buff[USBD_CUSTOM_IN_MAXPACKET_SIZE];

//  uint32_t hid2_protocol;
//  uint32_t hid2_set_idle;
//  uint32_t alt2_setting;
  
  uint8_t hid2_set_report[64];
  uint8_t hid2_get_report[64];
//  uint8_t hid2_state;
	
}keyboard_type;

/** @defgroup USB_custom_hid_class_request_code_definition
  * @{
  */

//typedef struct
//{
//  uint8_t g_rxhid_buff[USBD_CUSTOM_OUT_MAXPACKET_SIZE];
//  uint8_t g_txhid_buff[USBD_CUSTOM_IN_MAXPACKET_SIZE];

//  uint32_t hid_protocol;
//  uint32_t hid_set_idle;
//  uint32_t alt_setting;
//  
//  uint8_t hid_set_report[64];
//  uint8_t hid_get_report[64];
//  uint8_t hid_state;
//}custom_hid_type;


/** @defgroup USB_hid_class_exported_functions
  * @{
  */
extern usbd_class_handler keyboard_class_handler;

usb_sts_type usb_keyboard_class_send_report(void *udev, uint8_t *report, uint16_t len);
usb_sts_type custom_hid_class_send_report(void *udev, uint8_t *report, uint16_t len);
void usb_hid_keyboard_send_char(void *udev, uint8_t ascii_code);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif
