/**
  **************************************************************************
  * @file     keyboard_class.c
  * @brief    usb hid keyboard class type
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
#include "usbd_core.h"
#include "keyboard_class.h"
#include "keyboard_desc.h"
#include "at32_usb.h"

/** @addtogroup AT32F402_405_middlewares_usbd_class
  * @{
  */

/** @defgroup USB_keyboard_class
  * @brief usb device keyboard demo
  * @{
  */

/** @defgroup USB_keyboard_class_private_functions
  * @{
  */

static usb_sts_type class_init_handler(void *udev);
static usb_sts_type class_clear_handler(void *udev);
static usb_sts_type class_setup_handler(void *udev, usb_setup_type *setup);
static usb_sts_type class_ept0_tx_handler(void *udev);
static usb_sts_type class_ept0_rx_handler(void *udev);
static usb_sts_type class_in_handler(void *udev, uint8_t ept_num);
static usb_sts_type class_out_handler(void *udev, uint8_t ept_num);
static usb_sts_type class_sof_handler(void *udev);
static usb_sts_type class_event_handler(void *udev, usbd_event_type event);

static void usb_hid_buf_process(void *udev, uint8_t *report, uint16_t len);
keyboard_type keyboard_struct;
//custom_hid_type custom_hid_struct;
#define SHIFT 0x80
//asc码是下标，相当于媒介
const static unsigned char _asciimap[128+17+6] =
{
	/*0*/
  0x00,// NUL
  0x00,// SOH
  0x00,// STX
  0x00,// ETX
  0x00,// EOT
  0x00,// ENQ
  0x00,// ACK
  0x00,// BEL
  0x2A,// BS Backspace
  0x2B,// TAB  Tab
	/*9*/
	
  0x28,// LF  Enter
  0x00,// VT
  0x00,// FF
  0x00,// CR
  0x00,// SO
  0x00,// SI
  0x00,// DEL
  0x00,// DC1
  0x00,// DC2
  0x00,// DC3
	/*19*/	
	
  0x00,// DC4
  0x00,// NAK
  0x00,// SYN
  0x00,// ETB
  0x00,// CAN
  0x00,// EM
  0x00,// SUB
  0x00,// ESC
  0x00,// FS
  0x00,// GS
	/*29*/	
	
  0x00,// RS
  0x00,// US
	/*31*/

  0x2C,//  ' '
  0x1E|SHIFT,// !
  0x34|SHIFT,// "
  0x20|SHIFT,// #
  0x21|SHIFT,// $
  0x22|SHIFT,// %
  0x24|SHIFT,// &
  0x34,      // '
		/*39*/
	
  0x26|SHIFT,// (
  0x27|SHIFT,// )
  0x25|SHIFT,// *
  0x2E|SHIFT,// +
  0x36,// ,
  0x2D,// -
  0x37,// .
  0x38,// /
  0x27,// 0
  0x1E,// 1
  0x1F,// 2
  0x20,// 3
  0x21,// 4
  0x22,// 5
  0x23,// 6
  0x24,// 7
  0x25,// 8
  0x26,// 9
  0x33|SHIFT,// :
  0x33,      // ;
  0x36|SHIFT,// <
  0x2E,      // =
  0x37|SHIFT,// >
  0x38|SHIFT,// ?
  0x1F|SHIFT,// @
  0x04|SHIFT,// A
  0x05|SHIFT,// B
  0x06|SHIFT,// C
  0x07|SHIFT,// D
  0x08|SHIFT,// E
  0x09|SHIFT,// F
  0x0A|SHIFT,// G
  0x0B|SHIFT,// H
  0x0C|SHIFT,// I
  0x0D|SHIFT,// J
  0x0E|SHIFT,// K
  0x0F|SHIFT,// L
  0x10|SHIFT,// M
  0x11|SHIFT,// N
  0x12|SHIFT,// O
  0x13|SHIFT,// P
  0x14|SHIFT,// Q
  0x15|SHIFT,// R
  0x16|SHIFT,// S
  0x17|SHIFT,// T
  0x18|SHIFT,// U
  0x19|SHIFT,// V
  0x1A|SHIFT,// W
  0x1B|SHIFT,// X
  0x1C|SHIFT,// Y
  0x1D|SHIFT,// Z
  0x2F,      // [
  0x31,      // bslash
  0x30,      // ]
  0x23|SHIFT,// ^
  0x2D|SHIFT,// _
  0x35,      // `
  0x04,      // a
  0x05,      // b
  0x06,      // c
  0x07,      // d
  0x08,      // e
  0x09,      // f
  0x0A,      // g
  0x0B,      // h
  0x0C,      // i
  0x0D,      // j
  0x0E,      // k
  0x0F,      // l
  0x10,      // m
  0x11,      // n
  0x12,      // o
  0x13,      // p
  0x14,      // q
  0x15,      // r
  0x16,      // s
  0x17,      // t
  0x18,      // u
  0x19,      // v
  0x1A,      // w
  0x1B,      // x
  0x1C,      // y
  0x1D,      // z
  0x2f|SHIFT,//
  0x31|SHIFT,// |
  0x30|SHIFT,// }
  0x35|SHIFT,// ~
  0 , // DEL  127
	
	0x53,			 //Num Lock					128
	0x54,			 //   /
	0x55,			 //  *
	0x56,			 //  -
	0x57,			 //  +
	0x58,			 //ENTER
	0x59,			 //1 and End 
	0x5a,			 //2 and DownArrow
	0x5b,			 //3 and PageDn
	0x5c,			 //4 and LeftArrow
	0x5d,			 //5
	0x5e,			 //6 and RightArrow
	0x5f,			 //7 and Home
	0x60,			 //8 and UpArrow
	0x61,			 //9 and PageUp
	0x62,			 //0 and Insert
	0x63,			 //. andDelete   
	/*128+17-1=144*/
	
	0xe0,			 //LeftControl   145 UpArrow
	0xe1,			 //LeftShift     PgUp功能键
	0xe2,			 //LeftAlt       Insert功能键
	0xe4,			 //RightControl  字符'|'
	0xe5,			 //RightShift    鼠标右键
	0xe6			 // RightAlt      Shift
	/*144+6=150*/
};

/* usb device class handler */
usbd_class_handler keyboard_class_handler =
{
  class_init_handler,
  class_clear_handler,
  class_setup_handler,
  class_ept0_tx_handler,
  class_ept0_rx_handler,
  class_in_handler,
  class_out_handler,
  class_sof_handler,
  class_event_handler,
//	&custom_hid_struct,
  &keyboard_struct

};

/**
  * @brief  initialize usb endpoint
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_init_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  keyboard_type *pkeyboard = (keyboard_type *)pudev->class_handler->pdata;
//  custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;
  /* open hid in endpoint */
  usbd_ept_open(pudev, USBD_KEYBOARD_IN_EPT, EPT_INT_TYPE, USBD_KEYBOARD_IN_MAXPACKET_SIZE);

	  /* open custom hid in endpoint */
  usbd_ept_open(pudev, USBD_CUSTOM_HID_IN_EPT, EPT_INT_TYPE, USBD_CUSTOM_IN_MAXPACKET_SIZE);

  /* open custom hid out endpoint */
  usbd_ept_open(pudev, USBD_CUSTOM_HID_OUT_EPT, EPT_INT_TYPE, USBD_CUSTOM_OUT_MAXPACKET_SIZE);

  /* set out endpoint to receive status */
  usbd_ept_recv(pudev, USBD_CUSTOM_HID_OUT_EPT, pkeyboard->g_rxhid_buff, USBD_CUSTOM_OUT_MAXPACKET_SIZE);
	 
	pkeyboard->g_u8tx_completed = 1;
  return status;
}

/**
  * @brief  clear endpoint or other state
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_clear_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;

  /* close hid in endpoint */
  usbd_ept_close(pudev, USBD_KEYBOARD_IN_EPT);

	  /* close custom hid in endpoint */
  usbd_ept_close(pudev, USBD_CUSTOM_HID_IN_EPT);

  /* close custom hid out endpoint */
  usbd_ept_close(pudev, USBD_CUSTOM_HID_OUT_EPT);
	
  return status;
}


static usb_sts_type Keyboard_setup_handler(void *udev, usb_setup_type *setup)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  keyboard_type *pkeyboard = (keyboard_type *)pudev->class_handler->pdata;
//	custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;
  uint16_t len;
  uint8_t *buf;

  switch(setup->bmRequestType & USB_REQ_TYPE_RESERVED)
  {
    /* class request */
    case USB_REQ_TYPE_CLASS:
      switch(setup->bRequest)
      {
        case HID_REQ_SET_PROTOCOL:
          pkeyboard->hid_protocol = (uint8_t)setup->wValue;
          break;
        case HID_REQ_GET_PROTOCOL:
          usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->hid_protocol, 1);
          break;
        case HID_REQ_SET_IDLE:
          pkeyboard->hid_set_idle = (uint8_t)(setup->wValue >> 8);
          break;
        case HID_REQ_GET_IDLE:
          usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->hid_set_idle, 1);				
          break;
        case HID_REQ_SET_REPORT:
          pkeyboard->hid_state = HID_REQ_SET_REPORT;
          usbd_ctrl_recv(pudev, pkeyboard->hid_set_report, setup->wLength);
          break;
        default:
          usbd_ctrl_unsupport(pudev);
          break;
      }
      break;
    /* standard request */
    case USB_REQ_TYPE_STANDARD:
      switch(setup->bRequest)
      {
        case USB_STD_REQ_GET_DESCRIPTOR:
          if(setup->wValue >> 8 == HID_REPORT_DESC)
          {
							  len = MIN(USBD_KEYBOARD_SIZ_REPORT_DESC, setup->wLength);
								buf = (uint8_t *)g_usbd_keyboard_report;
								usbd_ctrl_send(pudev, (uint8_t *)buf, len);					
          }
          else if(setup->wValue >> 8 == HID_DESCRIPTOR_TYPE)
          {
            len = MIN(9, setup->wLength);
            buf = (uint8_t *)g_keyboard_usb_desc;
            usbd_ctrl_send(pudev, (uint8_t *)buf, len);
          }
          else
          {
            usbd_ctrl_unsupport(pudev);
          }
          break;
        case USB_STD_REQ_GET_INTERFACE:
          usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->alt_setting, 1);
          break;
        case USB_STD_REQ_SET_INTERFACE:
          pkeyboard->alt_setting = setup->wValue;
          break;
        case USB_STD_REQ_CLEAR_FEATURE:
          break;
        case USB_STD_REQ_SET_FEATURE:
          break;
        default:
          usbd_ctrl_unsupport(pudev);
          break;
      }
      break;
    default:
      usbd_ctrl_unsupport(pudev);
      break;
  }
  return status;
}


static usb_sts_type customhid_setup_handler(void *udev, usb_setup_type *setup)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  keyboard_type *pkeyboard = (keyboard_type *)pudev->class_handler->pdata;
//	custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;
  uint16_t len;
  uint8_t *buf;

  switch(setup->bmRequestType & USB_REQ_TYPE_RESERVED)
  {
    /* class request */
    case USB_REQ_TYPE_CLASS:
      switch(setup->bRequest)
      {
        case HID_REQ_SET_PROTOCOL:
          pkeyboard->hid_protocol = (uint8_t)setup->wValue;
//				pkeyboard->hid2_protocol = (uint8_t)setup->wValue;
          break;
        case HID_REQ_GET_PROTOCOL:
          usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->hid_protocol, 1);
//					usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->hid2_protocol, 1);
          break;
        case HID_REQ_SET_IDLE:
          pkeyboard->hid_set_idle = (uint8_t)(setup->wValue >> 8);
//				  pkeyboard->hid2_set_idle = (uint8_t)(setup->wValue >> 8);
          break;
        case HID_REQ_GET_IDLE:
          usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->hid_set_idle, 1);
//				usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->hid2_set_idle, 1);
				
          break;
        case HID_REQ_SET_REPORT:
          pkeyboard->hid_state = HID_REQ_SET_REPORT;
//          usbd_ctrl_recv(pudev, pkeyboard->hid_set_report, setup->wLength);
/*		   pkeyboard->hid2_state = HID_REQ_SET_REPORT;****/	
          usbd_ctrl_recv(pudev, pkeyboard->hid2_set_report, setup->wLength);
          break;
				case HID_REQ_GET_REPORT:
          usbd_ctrl_send(pudev, pkeyboard->hid2_get_report, setup->wLength);
          break;////
        default:
          usbd_ctrl_unsupport(pudev);
          break;
      }
      break;
    /* standard request */
    case USB_REQ_TYPE_STANDARD:
      switch(setup->bRequest)
      {
        case USB_STD_REQ_GET_DESCRIPTOR:
          if(setup->wValue >> 8 == HID_REPORT_DESC)
          {
							  len = MIN(USBD_CUSHID_SIZ_REPORT_DESC, setup->wLength);
								buf = (uint8_t *)g_usbd_custom_hid_report;
								usbd_ctrl_send(pudev, (uint8_t *)buf, len);						
          }
          else if(setup->wValue >> 8 == HID_DESCRIPTOR_TYPE)
          {
            len = MIN(9, setup->wLength);
            buf = (uint8_t *)g_keyboard_usb_desc;
            usbd_ctrl_send(pudev, (uint8_t *)buf, len);
          }
          else
          {
            usbd_ctrl_unsupport(pudev);
          }
          break;
        case USB_STD_REQ_GET_INTERFACE:
          usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->alt_setting, 1);
//          usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->alt2_setting, 1);
          break;
        case USB_STD_REQ_SET_INTERFACE:
          pkeyboard->alt_setting = setup->wValue;
//				 pkeyboard->alt2_setting = setup->wValue;
          break;
        case USB_STD_REQ_CLEAR_FEATURE:
          break;
        case USB_STD_REQ_SET_FEATURE:
          break;
        default:
          usbd_ctrl_unsupport(pudev);
          break;
      }
      break;
    default:
      usbd_ctrl_unsupport(pudev);
      break;
  }
  return status;
}




/**
  * @brief  usb device class setup request handler
  * @param  udev: to the structure of usbd_core_type
  * @param  setup: setup packet
  * @retval status of usb_sts_type
  */

static usb_sts_type class_setup_handler(void *udev, usb_setup_type *setup)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  switch(setup->bmRequestType & USB_REQ_RECIPIENT_MASK)
  {
    case USB_REQ_RECIPIENT_INTERFACE:
      if(setup->wIndex == 0x01)
      {
        customhid_setup_handler(udev, setup);
      }
      else
      {
        Keyboard_setup_handler(pudev, setup);
      }
      break;
    case USB_REQ_RECIPIENT_ENDPOINT:
      Keyboard_setup_handler(pudev, setup);
      break;

  }
  return status;


}



//static usb_sts_type class_setup_handler(void *udev, usb_setup_type *setup)
//{
//  usb_sts_type status = USB_OK;
//  usbd_core_type *pudev = (usbd_core_type *)udev;
//  keyboard_type *pkeyboard = (keyboard_type *)pudev->class_handler->pdata;
////	custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;
//  uint16_t len;
//  uint8_t *buf;

//  switch(setup->bmRequestType & USB_REQ_TYPE_RESERVED)
//  {
//    /* class request */
//    case USB_REQ_TYPE_CLASS:
//      switch(setup->bRequest)
//      {
//        case HID_REQ_SET_PROTOCOL:
//          pkeyboard->hid_protocol = (uint8_t)setup->wValue;
////				pkeyboard->hid2_protocol = (uint8_t)setup->wValue;
//          break;
//        case HID_REQ_GET_PROTOCOL:
//          usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->hid_protocol, 1);
////					usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->hid2_protocol, 1);
//          break;
//        case HID_REQ_SET_IDLE:
//          pkeyboard->hid_set_idle = (uint8_t)(setup->wValue >> 8);
////				  pkeyboard->hid2_set_idle = (uint8_t)(setup->wValue >> 8);
//          break;
//        case HID_REQ_GET_IDLE:
//          usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->hid_set_idle, 1);
////				usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->hid2_set_idle, 1);
//				
//          break;
//        case HID_REQ_SET_REPORT:
//          pkeyboard->hid_state = HID_REQ_SET_REPORT;
//          usbd_ctrl_recv(pudev, pkeyboard->hid_set_report, setup->wLength);
///*		   pkeyboard->hid2_state = HID_REQ_SET_REPORT;****/	
//          usbd_ctrl_recv(pudev, pkeyboard->hid2_set_report, setup->wLength);
//          break;
//				case HID_REQ_GET_REPORT:
//          usbd_ctrl_send(pudev, pkeyboard->hid2_get_report, setup->wLength);
//          break;////
//        default:
//          usbd_ctrl_unsupport(pudev);
//          break;
//      }
//      break;
//    /* standard request */
//    case USB_REQ_TYPE_STANDARD:
//      switch(setup->bRequest)
//      {
//        case USB_STD_REQ_GET_DESCRIPTOR:
//          if(setup->wValue >> 8 == HID_REPORT_DESC)
//          {
//						if(setup->wIndex==0x00)
//						{
//							  len = MIN(USBD_KEYBOARD_SIZ_REPORT_DESC, setup->wLength);
//								buf = (uint8_t *)g_usbd_keyboard_report;
//								usbd_ctrl_send(pudev, (uint8_t *)buf, len);
//						}
//						else
//						{
//							  len = MIN(USBD_CUSHID_SIZ_REPORT_DESC, setup->wLength);
//								buf = (uint8_t *)g_usbd_custom_hid_report;
//								usbd_ctrl_send(pudev, (uint8_t *)buf, len);
//						}
//						
//          }
//          else if(setup->wValue >> 8 == HID_DESCRIPTOR_TYPE)
//          {
//            len = MIN(9, setup->wLength);
//            buf = (uint8_t *)g_keyboard_usb_desc;
//            usbd_ctrl_send(pudev, (uint8_t *)buf, len);
//          }
//          else
//          {
//            usbd_ctrl_unsupport(pudev);
//          }
//          break;
//        case USB_STD_REQ_GET_INTERFACE:
//          usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->alt_setting, 1);
////          usbd_ctrl_send(pudev, (uint8_t *)&pkeyboard->alt2_setting, 1);
//          break;
//        case USB_STD_REQ_SET_INTERFACE:
//          pkeyboard->alt_setting = setup->wValue;
////				 pkeyboard->alt2_setting = setup->wValue;
//          break;
//        case USB_STD_REQ_CLEAR_FEATURE:
//          break;
//        case USB_STD_REQ_SET_FEATURE:
//          break;
//        default:
//          usbd_ctrl_unsupport(pudev);
//          break;
//      }
//      break;
//    default:
//      usbd_ctrl_unsupport(pudev);
//      break;
//  }
//  return status;
//}

/**
  * @brief  usb device class endpoint 0 in status stage complete
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_ept0_tx_handler(void *udev)
{
  usb_sts_type status = USB_OK;

  /* ...user code... */

  return status;
}

/**
  * @brief  usb device class endpoint 0 out status stage complete
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_ept0_rx_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  keyboard_type *pkeyboard = (keyboard_type *)pudev->class_handler->pdata;
//	custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;
	usb_setup_type *setup = &pudev->setup;
  uint32_t recv_len = usbd_get_recv_len(pudev, 0);
  /* ...user code... */
  if(setup->wIndex == 0x01)
  {
		if( pkeyboard->hid_state == HID_REQ_SET_REPORT)
		{
			/* hid buffer process */
			 usb_hid_buf_process(udev, pkeyboard->hid2_set_report, recv_len);
			pkeyboard->hid_state = 0;
		}
	}
  return status;
}

/**
  * @brief  usb device class transmision complete handler
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_num: endpoint number
  * @retval status of usb_sts_type
  */
static usb_sts_type class_in_handler(void *udev, uint8_t ept_num)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  keyboard_type *pkeyboard = (keyboard_type *)pudev->class_handler->pdata;

  /* ...user code...
    trans next packet data
  */
	if((ept_num & 0x7F) == (USBD_KEYBOARD_IN_EPT & 0x7F))
  {
    pkeyboard->g_u8tx_completed = 1;
  }
	else if((ept_num & 0x7F) == (USBD_CUSTOM_HID_IN_EPT & 0x7F))
  {
    usbd_flush_tx_fifo(udev, USBD_CUSTOM_HID_IN_EPT);
  }


  return status;
}

/**
  * @brief  usb device class endpoint receive data
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_num: endpoint number
  * @retval status of usb_sts_type
  */
static usb_sts_type class_out_handler(void *udev, uint8_t ept_num)
{
  usb_sts_type status = USB_OK;

	 usbd_core_type *pudev = (usbd_core_type *)udev;
	  keyboard_type *pkeyboard = (keyboard_type *)pudev->class_handler->pdata;
//  custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;

  /* get endpoint receive data length  */
  uint32_t recv_len = usbd_get_recv_len(pudev, ept_num);

if((ept_num & 0x7F) == (USBD_CUSTOM_HID_OUT_EPT & 0x7F))
  {
  /* hid buffer process */
  usb_hid_buf_process(udev, pkeyboard->g_rxhid_buff, recv_len);

  /* start receive next packet */
  usbd_ept_recv(pudev, USBD_CUSTOM_HID_OUT_EPT, pkeyboard->g_rxhid_buff, recv_len);
  }	
	
  return status;
}

/**
  * @brief  usb device class sof handler
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_sof_handler(void *udev)
{
  usb_sts_type status = USB_OK;

  /* ...user code... */

  return status;
}

/**
  * @brief  usb device class event handler
  * @param  udev: to the structure of usbd_core_type
  * @param  event: usb device event
  * @retval status of usb_sts_type
  */
static usb_sts_type class_event_handler(void *udev, usbd_event_type event)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  keyboard_type *pkeyboard = (keyboard_type *)pudev->class_handler->pdata;
  switch(event)
  {
    case USBD_RESET_EVENT:

      /* ...user code... */

      break;
    case USBD_SUSPEND_EVENT:
      pkeyboard->hid_suspend_flag = 1;
      /* ...user code... */

      break;
    case USBD_WAKEUP_EVENT:
      /* ...user code... */

      break;
    default:
      break;
  }
  return status;
}

/**
  * @brief  usb device class send report
  * @param  udev: to the structure of usbd_core_type
  * @param  report: report buffer
  * @param  len: report length
  * @retval status of usb_sts_type
  */
usb_sts_type usb_keyboard_class_send_report(void *udev, uint8_t *report, uint16_t len)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;

  if(usbd_connect_state_get(pudev) == USB_CONN_STATE_CONFIGURED)
    usbd_ept_send(pudev, USBD_KEYBOARD_IN_EPT, report, len);

  return status;
}


/**
  * @brief  usb device class send report
  * @param  udev: to the structure of usbd_core_type
  * @param  report: report buffer
  * @param  len: report length
  * @retval status of usb_sts_type
  */
usb_sts_type custom_hid_class_send_report(void *udev, uint8_t *report, uint16_t len)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;

  if(usbd_connect_state_get(pudev) == USB_CONN_STATE_CONFIGURED)
    usbd_ept_send(pudev, USBD_CUSTOM_HID_IN_EPT, report, len);

  return status;
}


/**
  * @brief  usb device class report function
  * @param  udev: to the structure of usbd_core_type
  * @param  op: operation
  * @retval none
  */
void usb_hid_keyboard_send_char(void *udev, uint8_t ascii_code)
{
  uint8_t key_data = 0;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  keyboard_type *pkeyboard = (keyboard_type *)pudev->class_handler->pdata;

  if(ascii_code > (128+17+6))
  {
    ascii_code = 0;
  }
  else
  {
    ascii_code = _asciimap[ascii_code];
    if(!ascii_code)
    {
      ascii_code = 0;
    }

    if(ascii_code & SHIFT)
    {
      key_data = 0x2;
      ascii_code &= 0x7F;
    }
  }

  if((pkeyboard->temp == ascii_code) && (ascii_code != 0))
  {
    pkeyboard->keyboard_buf[0] = 0;
    pkeyboard->keyboard_buf[2] = 0;
    usb_keyboard_class_send_report(udev, pkeyboard->keyboard_buf, 8);
  }
  else
  {
    pkeyboard->keyboard_buf[0] = key_data;
    pkeyboard->keyboard_buf[2] = ascii_code;
    usb_keyboard_class_send_report(udev, pkeyboard->keyboard_buf, 8);
  }
}

/**
  * @brief  usb device report function
  * @param  udev: to the structure of usbd_core_type
  * @param  report: report buffer
  * @param  len: report length
  * @retval none
  */
static void usb_hid_buf_process(void *udev, uint8_t *report, uint16_t len)
{
  uint32_t i_index;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  keyboard_type *pkeyboard = (keyboard_type *)pudev->class_handler->pdata;
	
	
  switch(report[0])
  {
    case HID_REPORT_ID_2:
      if(pkeyboard->g_rxhid_buff[1] == 0)
      {
        //at32_led_off(LED2);
      }
      else
      {
        //at32_led_on(LED2);
      }
      break;
    case HID_REPORT_ID_3:
      if(pkeyboard->g_rxhid_buff[1] == 0)
      {
        //at32_led_off(LED3);
      }
      else
      {
        //at32_led_on(LED3);
      }
      break;
    case HID_REPORT_ID_4:
      if(pkeyboard->g_rxhid_buff[1] == 0)
      {
        //at32_led_off(LED4);
      }
      else
      {
        //at32_led_on(LED4);
      }
      break;
		/******自定义HID数据接收*****/
    case HID_REPORT_ID_6:
      for(i_index = 0; i_index < len; i_index ++)
      {
        pkeyboard->g_txhid_buff[i_index] = report[i_index];
      }
//      usbd_ept_send(pudev, USBD_CUSTOM_HID_IN_EPT, pkeyboard->g_txhid_buff, len);
			//更新显示
			//num_map(pkeyboard->g_rxhid_buff[1],pkeyboard->g_rxhid_buff[2],pkeyboard->g_rxhid_buff[3],pkeyboard->g_rxhid_buff[4],pkeyboard->g_rxhid_buff[5],pkeyboard->g_rxhid_buff[6]);
	  At32UsbReceive(pkeyboard->g_txhid_buff, len);
      break;
    default:
      break;
  }

}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

