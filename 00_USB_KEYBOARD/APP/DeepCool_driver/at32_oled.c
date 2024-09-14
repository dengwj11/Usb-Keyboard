#include "at32_oled.h"
#include "at32_iic.h"
#include "i2c_application.h"
//#include <string.h>
#include "oledfont.h"
#include "delay.h"
#include "at32_spi.h"

#if OLED_IIC_EN==1

#if OLED_SOFT_IIC_EN==1
		/*引脚配置*/
	#define OLED_W_SCL(x)		gpio_bits_write(GPIOC, GPIO_PINS_6, (confirm_state)x)//FALSE = 0, TRUE = !FALSE
	#define OLED_W_SDA(x)		gpio_bits_write(GPIOC, GPIO_PINS_7, (confirm_state)x)

	/*引脚初始化*/
	void OLED_I2C_Init(void)
	{
		 gpio_init_type gpio_init_structure;
	//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			 crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
			/* set default parameter */
		gpio_default_para_init(&gpio_init_structure);
	//	GPIO_InitTypeDef GPIO_InitStructure;
	// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	// 	GPIO_Init(GPIOB, &GPIO_InitStructure);

			/* configure i2c pins: scl */
		gpio_init_structure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
		gpio_init_structure.gpio_mode           = GPIO_MODE_OUTPUT;
		gpio_init_structure.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;//开漏
		gpio_init_structure.gpio_pull           = GPIO_PULL_UP;

		gpio_init_structure.gpio_pins           = GPIO_PINS_6;
		gpio_init(GPIOC, &gpio_init_structure);

		/* configure i2c pins: sda */
		gpio_init_structure.gpio_pins           = GPIO_PINS_7;
		gpio_init(GPIOC, &gpio_init_structure);
	//	 
	//	    /* gpio configuration */
	//    gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE6, GPIO_MUX_3);
	//    gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE7, GPIO_MUX_3);
			
		OLED_W_SCL(1);
		OLED_W_SDA(1);
	}

	/**
		* @brief  I2C开始
		* @param  无
		* @retval 无
		*/
	void OLED_I2C_Start(void)
	{
		OLED_W_SDA(1);
		OLED_W_SCL(1);
		OLED_W_SDA(1);delay_us(4); 
		delay_us(10); 
		OLED_W_SDA(0);
		delay_us(10);
		OLED_W_SCL(0);
	}

	/**
		* @brief  I2C停止
		* @param  无
		* @retval 无
		*/
	void OLED_I2C_Stop(void)
	{
		OLED_W_SDA(0);
		OLED_W_SCL(1);
		 delay_us(10); 
		OLED_W_SDA(1);
		delay_us(10);
		OLED_W_SCL(1); OLED_W_SDA(1);
	}

	/**
		* @brief  I2C发送一个字节
		* @param  Byte 要发送的一个字节
		* @retval 无
		*/
	void OLED_I2C_SendByte(uint8_t Byte)
	{
		uint8_t i;
		for (i = 0; i < 8; i++)
		{
			OLED_W_SDA((confirm_state)(Byte & (0x80 >> i)));
			delay_us(3); 
			OLED_W_SCL(1);
			delay_us(5);
			delay_us(2); 
			OLED_W_SCL(0);
			delay_us(2);
		}
		OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
		delay_us(5);
		OLED_W_SCL(0);
		 delay_us(3);
	}

	/**
		* @brief  OLED写命令
		* @param  Command 要写入的命令
		* @retval 无
		*/
	void OLED_WriteCommand(uint8_t Command)
	{
		OLED_I2C_Start();
		OLED_I2C_SendByte(0x78);		//从机地址
		OLED_I2C_SendByte(0x00);		//写命令
		OLED_I2C_SendByte(Command); 
		OLED_I2C_Stop();
	}

	/**
		* @brief  OLED写数据
		* @param  Data 要写入的数据
		* @retval 无
		*/
	void OLED_WriteData(uint8_t Data)
	{
		OLED_I2C_Start();
		OLED_I2C_SendByte(0x78);		//从机地址
		OLED_I2C_SendByte(0x40);		//写数据
		OLED_I2C_SendByte(Data);
		OLED_I2C_Stop();
	}

#else
		uint8_t dat[2]={0x40};
		uint8_t cmd[2]={0x00};
		/**
		* @brief  OLED写命令
		* @param  Command 要写入的命令
		* @retval 无
		*/
	void OLED_WriteCommand(uint8_t Command)
	{
//		OLED_I2C_Start();
//		OLED_I2C_SendByte(0x78);		//从机地址
//		OLED_I2C_SendByte(0x00);		//写命令
//		OLED_I2C_SendByte(Command); 
//		OLED_I2C_Stop();
		cmd[1]=Command;
	  i2c_master_transmit(&hi2cx, I2Cx_ADDRESS, cmd, 2, I2C_TIMEOUT);
	}

	/**
		* @brief  OLED写数据
		* @param  Data 要写入的数据
		* @retval 无
		*/
	void OLED_WriteData(uint8_t Data)
	{
//		OLED_I2C_Start();
//		OLED_I2C_SendByte(0x78);		//从机地址
//		OLED_I2C_SendByte(0x40);		//写数据
//		OLED_I2C_SendByte(Data);
//		OLED_I2C_Stop();
		dat[1]=Data;
	  i2c_master_transmit(&hi2cx, I2Cx_ADDRESS, dat, 2, I2C_TIMEOUT);
	}
	
#endif

#endif

#if OLED_SPI_EN==1

/***********************************************************************
      函数说明：软件spi写入数据
      入口数据：dat  要写入数据
      返回值：  无
************************************************************************/
#if OLED_SOFT_SPI_EN == 1
void Write_byte(unsigned char dat)
{
	delay_us(10);
	unsigned char i;
		for(i=0;i<8;i++)
		{
			SCL=0;
			//_nop_();
			delay_us(10);
			if((dat<<i)&0x80)
			{
				SDA=1;
				delay_us(10);
			}
			else
			{
				SDA=0;
				delay_us(10);
			}
			SCL=1;
			delay_us(10);
		}
}

#else

/***********************************************************************
      函数说明：硬件spi写入数据
      入口数据：dat  要写入数据
      返回值：  无
************************************************************************/
void Write_byte(unsigned char dat)
{
    while(spi_i2s_flag_get(SPI1, SPI_I2S_TDBE_FLAG) == RESET); //等待上一数据发送完成
	    spi_i2s_data_transmit(SPI1,dat);
//		while(spi_i2s_flag_get(SPI1, SPI_I2S_RDBF_FLAG) == RESET); 
    while(spi_i2s_flag_get(SPI1, SPI_I2S_BF_FLAG) == TRUE);//忙等待

} 

#endif
void OLED_WriteCommand(unsigned char dat)
{
	CS=0;
	delay_us(10);
	DC=0;//选择命令
	delay_us(10);
	//SCL=0;
	Write_byte(dat);
	delay_us(10);
	CS=1;
	delay_us(10);
	//RS=1;
}

void OLED_WriteData(unsigned char dat)
{
	CS=0;
	delay_us(10);
	DC=1;//选择数据
	delay_us(10);
	//SCL=0;
	Write_byte(dat);
	CS=1;
	delay_us(10);
	//RS=0;

}

#endif


/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~OLED_HEIGHT/8-1
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~OLED_WIDTH-1
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{

	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位

		OLED_WriteCommand(0xB0 | Y);					//设置Y位置

}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < OLED_HEIGHT/8 /*8*/; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < OLED_WIDTH/*128*/; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
* @brief  OLED显示一个字符(h:w=16:8)
  * @param  Line 行位置，范围：1~OLED_HEIGHT/16       4
  * @param  Column 列位置，范围：1~OLED_WIDTH/8       16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~OLED_HEIGHT/16       4
  * @param  Column 起始列位置，范围：1~OLED_WIDTH/8       16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~OLED_HEIGHT/16       4
  * @param  Column 起始列位置，范围：1~OLED_WIDTH/8       16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~OLED_HEIGHT/16       4
  * @param  Column 起始列位置，范围：1~OLED_WIDTH/8       16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
#if OLED_IIC_EN==1
#if OLED_SOFT_IIC_EN==1
	OLED_I2C_Init();			//端口初始化
#else
	I2cInit();
#endif
#endif
	
#if OLED_SPI_EN==1
#if OLED_SOFT_SPI_EN==1
	SpiInit();			//端口初始化
#else
	spi_config();
#endif
#endif
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	
#if OLED_SH1107_EN==1

	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x7F);  //根据高度的像素值
	OLED_WriteCommand(0x20);	//页地址模式
#else 

	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);  //根据高度的像素值
	

#endif

	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
//	OLED_WriteCommand(0xDC);	//设置显示偏移
//	OLED_WriteCommand(0x00);		
	OLED_WriteCommand(0x40);	//设置显示开始行


	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}


