/**
  **************************************************************************
  * @file     keyboard_desc.h
  * @brief    usb keyboard descriptor header file
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
#ifndef __KEYBOARD_DESC_H
#define __KEYBOARD_DESC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "keyboard_class.h"
#include "usbd_core.h"

/** @addtogroup AT32F402_405_middlewares_usbd_class
  * @{
  */

/** @addtogroup USB_keyboard_desc
  * @{
  */

/** @defgroup USB_keyboard_desc_definition
  * @{
  */

/**
  * @brief usb bcd number define
  */
#define KEYBOARD_BCD_NUM                      0x0111

/**
  * @brief usb vendor id and product id define
  */
#define USBD_KEYBOARD_VENDOR_ID          0x2A3A
#define USBD_KEYBOARD_PRODUCT_ID         0x8040

/**
  * @brief usb descriptor size define
  */
#define USBD_KEYBOARD_CONFIG_DESC_SIZE    66   //34
#define USBD_KEYBOARD_SIZ_REPORT_DESC     120	//63+2+49+6
#define USBD_CUSHID_SIZ_REPORT_DESC      126
#define USBD_KEYBOARD_SIZ_STRING_LANGID   4
#define USBD_KEYBOARD_SIZ_STRING_SERIAL   0x1A

/**
  * @brief usb string define(vendor, product configuration, interface)
  */
#define USBD_KEYBOARD_DESC_MANUFACTURER_STRING    "DeeCool"
#define USBD_KEYBOARD_DESC_PRODUCT_STRING         "Cool-Keyboard"
#define USBD_KEYBOARD_DESC_CONFIGURATION_STRING   "DeepCool Keyboard Config"
#define USBD_KEYBOARD_DESC_INTERFACE_STRING       "DeepCool Keyboard Interface"

/**
  * @brief usb hid endpoint interval define
  */
#define KEYBOARD_BINTERVAL_TIME                0x0A
#define KEYBOARD_HS_BINTERVAL_TIME             0x01
#define CUSHID_BINTERVAL_TIME            			 0x0A

/**
  * @brief usb hid report id define
  */
#define HID_REPORT_ID_1                   0x01
#define HID_REPORT_ID_2                   0x02
#define HID_REPORT_ID_3                   0x03
#define HID_REPORT_ID_4                   0x04
#define HID_REPORT_ID_5                   0x05
#define HID_REPORT_ID_6                   0x10

/**
  * @brief usb mcu id address deine
  */
#define         MCU_ID1                   (0x1FFFF7E8)
#define         MCU_ID2                   (0x1FFFF7EC)
#define         MCU_ID3                   (0x1FFFF7F0)

/**
  * @}
  */
extern uint8_t g_usbd_keyboard_report[USBD_KEYBOARD_SIZ_REPORT_DESC];
extern uint8_t g_usbd_custom_hid_report[USBD_CUSHID_SIZ_REPORT_DESC];
extern uint8_t g_keyboard_usb_desc[9];
extern usbd_desc_handler keyboard_desc_handler;

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


