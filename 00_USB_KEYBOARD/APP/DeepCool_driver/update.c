#include "at32f402_405.h"               // Device header
#include "update.h"  

void GetFirmWareVersion(void)
{
	volatile unsigned int tmp_version = FIRMWARE_VERSION;
	unsigned char tmp_usb_send_buffer[5];
	
	tmp_usb_send_buffer[0] = 0x03;
	tmp_usb_send_buffer[1] = (tmp_version >> 24) & 0xFF; 
	tmp_usb_send_buffer[2] = (tmp_version >> 16) & 0xFF; 
	tmp_usb_send_buffer[3] =(tmp_version >> 8) & 0xFF;  
	tmp_usb_send_buffer[4] = tmp_version & 0xFF;
	
	FramingData(tmp_usb_send_buffer, 5);
}
