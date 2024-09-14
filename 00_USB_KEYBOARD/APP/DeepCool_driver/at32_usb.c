#include "at32_usb.h"
#include "soft_timer.h"
//#include "keyboard_desc.h"

/* usb global struct define */
otg_core_type otg_core_struct;
uint8_t report_buf[USBD_CUSTOM_IN_MAXPACKET_SIZE];
USB_PACKET_STRUCT g_usb_packet = {0};
unsigned char g_usb_receive_flag = 0;


/**
  * @brief  对要发送的数据进行包封装.
  * @param  str 要发送的数据首地址
  * @param  len 要发送的数据长度
  * @retval none
  */
void FramingData(unsigned char* str, unsigned short len)
{
	unsigned char offset = 0;
	unsigned char g_frame_buffer[128];
	unsigned char check_sum_value = 0;
	
	g_frame_buffer[offset++] = FRAME_HEAD;
	g_frame_buffer[offset++] = DEVICE_TYPE;
	g_frame_buffer[offset++] = DEVICE_NUMBER;
	g_frame_buffer[offset++] = len;
	
	for(unsigned char i = 0; i < len; i++)
	{
		g_frame_buffer[offset++] = str[i];
	}
	
	for(unsigned char i = 0; i < len + 4; i++)
	{
		check_sum_value += g_frame_buffer[i];
	}
	
	g_frame_buffer[offset++] = check_sum_value;
	
	g_frame_buffer[offset++] = FRAME_TAIL;
	
	At32UsbSend(g_frame_buffer, 64);
}


/**
  * @brief  拆包确认无误后，数据保存处理函数.
  * @param  start_position 帧头位置
  * @retval none
  */
void EventDispose(unsigned char start_position)
{
	if(g_usb_packet.usb_rx_buf[start_position + 4] == FUNCTION_CODE_CHANGE_KEY)
	{
     keyboard_map_change(g_usb_packet.usb_rx_buf[start_position + 5],g_usb_packet.usb_rx_buf[start_position + 6],g_usb_packet.usb_rx_buf[start_position + 7]);
	}
//	else if(g_usb_packet.usb_rx_buf[start_position + 4] == FUNCTION_CODE_SWITCH_ANIMATION)
//	{
//		if(g_animation_start_once_flag == ANIMATION_NOT_START)
//		{
//			g_animation_finish_flag = ANIMATION_NOT_FINISH;
//			g_screen_direction = g_usb_packet.usb_rx_buf[start_position + 6];
//			g_cur_show_mode  = STARTUP_MODE;
//			g_normal_show_count = 15;
//		}	
//	}
//	else if(g_usb_packet.usb_rx_buf[start_position + 4] == FUNCTION_CODE_SWITCH_BRIGHTNESS)
//	{
//		i2c_master_transmit(&hi2cx, I2Cx_ADDRESS, (uint8_t*)g_pwm_value + g_usb_packet.usb_rx_buf[start_position + 5], 1, I2C_TIMEOUT);
//	}
	else if(g_usb_packet.usb_rx_buf[start_position + 4] == FUNCTION_CODE_GET_VERSION)
	{
		GetFirmWareVersion();
	}
}

/**
  * @brief  拆包判断处理函数.
  * @param  none
  * @retval none
  */
void PacketDispose(void)
{
	unsigned char find_flag = 0;
	unsigned char start_position = 0;
	unsigned char end_position = 0;
	unsigned char data_domain_len = 0;
	unsigned char check_code = 0;
	
	if(g_usb_packet.usb_rx_len < FRAME_MIN_LEN)
	{
		g_usb_receive_flag = 0;
		return;
	}
	
	for(unsigned char i = 0; i < g_usb_packet.usb_rx_len; i++)
	{
		if(g_usb_packet.usb_rx_buf[i] == FRAME_HEAD)
		{
			find_flag = 1;
			start_position = i;
			break;
		}
	}
	
	if((find_flag == 0) || (g_usb_packet.usb_rx_len -  start_position < FRAME_MIN_LEN)) 
	{
		g_usb_receive_flag = 0;
		return;
	}
	
	data_domain_len = g_usb_packet.usb_rx_buf[start_position + 3];
	
	end_position = start_position + 5 + data_domain_len;
	
	if(end_position >= g_usb_packet.usb_rx_len)
	{
		g_usb_receive_flag = 0;
		return;
	}
	
	if(g_usb_packet.usb_rx_buf[end_position] != FRAME_TAIL)
	{
		g_usb_receive_flag = 0;
		return;
	}
	
	for(unsigned char i = start_position; i < end_position - 1; i++)
		check_code += g_usb_packet.usb_rx_buf[i];
	
	if(check_code != g_usb_packet.usb_rx_buf[end_position - 1])
	{
		g_usb_receive_flag = 0;
		return;
	}
	
	EventDispose(start_position);
	
	g_usb_receive_flag = 0;
}

/**
  * @brief  向上位机发送数据.
  * @param  none
  * @retval none
  */
void At32UsbSend(uint8_t* send_buffer, uint16_t len)
{
	otg_core_type* otgdev = &otg_core_struct;
	usbd_core_type *pudev = &otgdev->dev;
    keyboard_type *pcshid = (keyboard_type *)pudev->class_handler->pdata;
	
	if(len > USBD_CUSTOM_IN_MAXPACKET_SIZE)
		return;
	
	pcshid->g_txhid_buff[0] = HID_REPORT_ID_6;
	for(unsigned int i = 1; i < len + 1; i++)
	{
		pcshid->g_txhid_buff[i] = send_buffer[i - 1];
	}
	
	if(usbd_connect_state_get(pudev) == USB_CONN_STATE_CONFIGURED)
		usbd_ept_send(pudev, USBD_CUSTOM_HID_IN_EPT, pcshid->g_txhid_buff, len);
}


/**
  * @brief  当接收到上位机发来的数据，在缓冲区对数据进行处理.
  * @param  receive_buffer 缓冲区数据指针
  * @param  len 缓冲区数据长度
  * @retval none
  */
void At32UsbReceive(uint8_t* receive_buffer, unsigned char len)
{
	if(0 == g_usb_receive_flag)
	{
		g_usb_receive_flag = 1;
		
		g_usb_packet.usb_rx_len = 0;
		memset(g_usb_packet.usb_rx_buf, 0, USBD_CUSTOM_IN_MAXPACKET_SIZE);
		g_usb_packet.usb_rx_len = len;
		memcpy(g_usb_packet.usb_rx_buf, receive_buffer, len);
		
		SoftTimerActiveEvent(PacketDispose, 0, 0);
	}
}


/**
  * @brief  keyboard send string
  * @param  string: send string详见报告描述符
//  * @param  DUOMEITI: 1 用了标准键盘
  * @retval none
  */
void keyboard_send_string(void *udev, uint8_t *string,uint8_t DUOMEITI)
{
//uint8_t key_data = 0;
  uint8_t index = 0;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  keyboard_type *pkeyboard = (keyboard_type *)pudev->class_handler->pdata;
	
		memset(pkeyboard->keyboard_buf,0x00,15+1);
	 switch(DUOMEITI)
  {
    case 1:
			pkeyboard->keyboard_buf[0]=0x01;
			for(index=0;index<15;index++)
			{
				if(index==1) {continue;}
				pkeyboard->keyboard_buf[index+1]=string[index];
				
			}
      break;
    case 2:
			pkeyboard->keyboard_buf[0]=0x02;
			pkeyboard->keyboard_buf[1]=string[15];
      break;
    default:
      break;
  }		
		if(pkeyboard->g_u8tx_completed == 1)
		{
			pkeyboard->g_u8tx_completed = 0;

			usb_keyboard_class_send_report(udev, pkeyboard->keyboard_buf, 15+1);
		}
//		string[15]=0x00;
//		pkeyboard->keyboard_buf[0]=0x00;
//		pkeyboard->keyboard_buf[1]=0;
}




/**
  * @brief  usb 48M clock select
  * @param  clk_s:USB_CLK_HICK, USB_CLK_HEXT
  * @retval none
  */
void usb_clock48m_select(usb_clk48_s clk_s)
{
  if(clk_s == USB_CLK_HICK)
  {
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);

    /* enable the acc calibration ready interrupt */
    crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);

    /* update the c1\c2\c3 value */
    acc_write_c1(7980);
    acc_write_c2(8000);
    acc_write_c3(8020);

    /* open acc calibration */
    acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);
  }
  else
  {
    crm_pllu_output_set(TRUE);
    /* wait till pll is ready */
    while(crm_flag_get(CRM_PLLU_STABLE_FLAG) != SET)
    {
    }
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_PLLU);
  }
}

/**
  * @brief  this function config gpio.
  * @param  none
  * @retval none
  */
void usb_gpio_config(void)
{
  gpio_init_type gpio_init_struct;

  crm_periph_clock_enable(OTG_PIN_GPIO_CLOCK, TRUE);
  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

  crm_periph_clock_enable(OTG_PIN_GPIO_CLOCK, TRUE);
#ifdef USB_SOF_OUTPUT_ENABLE
  crm_periph_clock_enable(OTG_PIN_SOF_GPIO_CLOCK, TRUE);
  gpio_init_struct.gpio_pins = OTG_PIN_SOF;
  gpio_init(OTG_PIN_SOF_GPIO, &gpio_init_struct);
  gpio_pin_mux_config(OTG_PIN_SOF_GPIO, OTG_PIN_SOF_SOURCE, OTG_PIN_MUX);
#endif

  /* use vbus pin */
#ifndef USB_VBUS_IGNORE
  gpio_init_struct.gpio_pins = OTG_PIN_VBUS;
  gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
  gpio_pin_mux_config(OTG_PIN_GPIO, OTG_PIN_VBUS_SOURCE, OTG_PIN_MUX);
  gpio_init(OTG_PIN_GPIO, &gpio_init_struct);
#endif


}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
void at32_usb_init(void)
{
  __IO uint32_t delay_index = 0;
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  /* usb gpio config */
  usb_gpio_config();

  /* enable otg clock */
  crm_periph_clock_enable(OTG_CLOCK, TRUE);

  /* select usb 48m clcok source */
  usb_clock48m_select(USB_CLK_HEXT);

  /* enable otg irq */
  nvic_irq_enable(OTG_IRQ, 0, 0);

  /* init usb */
  usbd_init(&otg_core_struct,
            USB_SPEED_CORE_ID,
            USB_ID,
            &keyboard_class_handler,
            &keyboard_desc_handler);
						
//  while(1)
//  {
//    if(at32_button_press() == USER_BUTTON)
//    {
//      if(usbd_connect_state_get(&otg_core_struct.dev) == USB_CONN_STATE_CONFIGURED)
//      {
//        keyboard_send_string(&otg_core_struct.dev, (uint8_t *)" Keyboard Demo\r\n", 16);
//      }
//      /* remote wakeup */
//      if(usbd_connect_state_get(&otg_core_struct.dev) == USB_CONN_STATE_SUSPENDED
//        && (otg_core_struct.dev.remote_wakup == 1))
//      {
//        usbd_remote_wakeup(&otg_core_struct.dev);
//      }
//    }



 }



/**
  * @brief  this function handles otgfs interrupt.
  * @param  none
  * @retval none
  */
void OTG_IRQ_HANDLER(void)
{
//	  if(exint_flag_get(OTG_WKUP_EXINT_LINE))
//  {
//    exint_flag_clear(OTG_WKUP_EXINT_LINE);
//  }
  usbd_irq_handler(&otg_core_struct);
}

/**
  * @brief  usb delay millisecond function.
  * @param  ms: number of millisecond delay
  * @retval none
  */
void usb_delay_ms(uint32_t ms)
{

  /* user can define self delay function */
  delay_ms(ms);
}

/**
  * @brief  usb delay microsecond function.
  * @param  us: number of microsecond delay
  * @retval none
  */
void usb_delay_us(uint32_t us)
{
  delay_us(us);
}

/**
  * @}
  */

/**
  * @}
  */
