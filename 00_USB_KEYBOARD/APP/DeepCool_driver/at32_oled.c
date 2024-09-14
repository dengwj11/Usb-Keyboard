#include "at32_oled.h"
#include "at32_iic.h"
#include "i2c_application.h"
//#include <string.h>
#include "oledfont.h"
#include "delay.h"
#include "at32_spi.h"

#if OLED_IIC_EN==1

#if OLED_SOFT_IIC_EN==1
		/*��������*/
	#define OLED_W_SCL(x)		gpio_bits_write(GPIOC, GPIO_PINS_6, (confirm_state)x)//FALSE = 0, TRUE = !FALSE
	#define OLED_W_SDA(x)		gpio_bits_write(GPIOC, GPIO_PINS_7, (confirm_state)x)

	/*���ų�ʼ��*/
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
		gpio_init_structure.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;//��©
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
		* @brief  I2C��ʼ
		* @param  ��
		* @retval ��
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
		* @brief  I2Cֹͣ
		* @param  ��
		* @retval ��
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
		* @brief  I2C����һ���ֽ�
		* @param  Byte Ҫ���͵�һ���ֽ�
		* @retval ��
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
		OLED_W_SCL(1);	//�����һ��ʱ�ӣ�������Ӧ���ź�
		delay_us(5);
		OLED_W_SCL(0);
		 delay_us(3);
	}

	/**
		* @brief  OLEDд����
		* @param  Command Ҫд�������
		* @retval ��
		*/
	void OLED_WriteCommand(uint8_t Command)
	{
		OLED_I2C_Start();
		OLED_I2C_SendByte(0x78);		//�ӻ���ַ
		OLED_I2C_SendByte(0x00);		//д����
		OLED_I2C_SendByte(Command); 
		OLED_I2C_Stop();
	}

	/**
		* @brief  OLEDд����
		* @param  Data Ҫд�������
		* @retval ��
		*/
	void OLED_WriteData(uint8_t Data)
	{
		OLED_I2C_Start();
		OLED_I2C_SendByte(0x78);		//�ӻ���ַ
		OLED_I2C_SendByte(0x40);		//д����
		OLED_I2C_SendByte(Data);
		OLED_I2C_Stop();
	}

#else
		uint8_t dat[2]={0x40};
		uint8_t cmd[2]={0x00};
		/**
		* @brief  OLEDд����
		* @param  Command Ҫд�������
		* @retval ��
		*/
	void OLED_WriteCommand(uint8_t Command)
	{
//		OLED_I2C_Start();
//		OLED_I2C_SendByte(0x78);		//�ӻ���ַ
//		OLED_I2C_SendByte(0x00);		//д����
//		OLED_I2C_SendByte(Command); 
//		OLED_I2C_Stop();
		cmd[1]=Command;
	  i2c_master_transmit(&hi2cx, I2Cx_ADDRESS, cmd, 2, I2C_TIMEOUT);
	}

	/**
		* @brief  OLEDд����
		* @param  Data Ҫд�������
		* @retval ��
		*/
	void OLED_WriteData(uint8_t Data)
	{
//		OLED_I2C_Start();
//		OLED_I2C_SendByte(0x78);		//�ӻ���ַ
//		OLED_I2C_SendByte(0x40);		//д����
//		OLED_I2C_SendByte(Data);
//		OLED_I2C_Stop();
		dat[1]=Data;
	  i2c_master_transmit(&hi2cx, I2Cx_ADDRESS, dat, 2, I2C_TIMEOUT);
	}
	
#endif

#endif

#if OLED_SPI_EN==1

/***********************************************************************
      ����˵�������spiд������
      ������ݣ�dat  Ҫд������
      ����ֵ��  ��
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
      ����˵����Ӳ��spiд������
      ������ݣ�dat  Ҫд������
      ����ֵ��  ��
************************************************************************/
void Write_byte(unsigned char dat)
{
    while(spi_i2s_flag_get(SPI1, SPI_I2S_TDBE_FLAG) == RESET); //�ȴ���һ���ݷ������
	    spi_i2s_data_transmit(SPI1,dat);
//		while(spi_i2s_flag_get(SPI1, SPI_I2S_RDBF_FLAG) == RESET); 
    while(spi_i2s_flag_get(SPI1, SPI_I2S_BF_FLAG) == TRUE);//æ�ȴ�

} 

#endif
void OLED_WriteCommand(unsigned char dat)
{
	CS=0;
	delay_us(10);
	DC=0;//ѡ������
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
	DC=1;//ѡ������
	delay_us(10);
	//SCL=0;
	Write_byte(dat);
	CS=1;
	delay_us(10);
	//RS=0;

}

#endif


/**
  * @brief  OLED���ù��λ��
  * @param  Y �����Ͻ�Ϊԭ�㣬���·�������꣬��Χ��0~OLED_HEIGHT/8-1
  * @param  X �����Ͻ�Ϊԭ�㣬���ҷ�������꣬��Χ��0~OLED_WIDTH-1
  * @retval ��
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{

	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//����Xλ�ø�4λ
	OLED_WriteCommand(0x00 | (X & 0x0F));			//����Xλ�õ�4λ

		OLED_WriteCommand(0xB0 | Y);					//����Yλ��

}

/**
  * @brief  OLED����
  * @param  ��
  * @retval ��
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
* @brief  OLED��ʾһ���ַ�(h:w=16:8)
  * @param  Line ��λ�ã���Χ��1~OLED_HEIGHT/16       4
  * @param  Column ��λ�ã���Χ��1~OLED_WIDTH/8       16
  * @param  Char Ҫ��ʾ��һ���ַ�����Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//���ù��λ�����ϰ벿��
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//��ʾ�ϰ벿������
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//���ù��λ�����°벿��
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//��ʾ�°벿������
	}
}

/**
  * @brief  OLED��ʾ�ַ���
  * @param  Line ��ʼ��λ�ã���Χ��1~OLED_HEIGHT/16       4
  * @param  Column ��ʼ��λ�ã���Χ��1~OLED_WIDTH/8       16
  * @param  String Ҫ��ʾ���ַ�������Χ��ASCII�ɼ��ַ�
  * @retval ��
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
  * @brief  OLED�η�����
  * @retval ����ֵ����X��Y�η�
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
  * @brief  OLED��ʾ���֣�ʮ���ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~OLED_HEIGHT/16       4
  * @param  Column ��ʼ��λ�ã���Χ��1~OLED_WIDTH/8       16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~4294967295
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
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
  * @brief  OLED��ʾ���֣�ʮ���ƣ�����������
  * @param  Line ��ʼ��λ�ã���Χ��1~OLED_HEIGHT/16       4
  * @param  Column ��ʼ��λ�ã���Χ��1~OLED_WIDTH/8       16
  * @param  Number Ҫ��ʾ�����֣���Χ��-2147483648~2147483647
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
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
  * @brief  OLED��ʾ���֣�ʮ�����ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~0xFFFFFFFF
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~8
  * @retval ��
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
  * @brief  OLED��ʾ���֣������ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~16
  * @retval ��
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
  * @brief  OLED��ʼ��
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//�ϵ���ʱ
	{
		for (j = 0; j < 1000; j++);
	}
#if OLED_IIC_EN==1
#if OLED_SOFT_IIC_EN==1
	OLED_I2C_Init();			//�˿ڳ�ʼ��
#else
	I2cInit();
#endif
#endif
	
#if OLED_SPI_EN==1
#if OLED_SOFT_SPI_EN==1
	SpiInit();			//�˿ڳ�ʼ��
#else
	spi_config();
#endif
#endif
	
	OLED_WriteCommand(0xAE);	//�ر���ʾ
	
	OLED_WriteCommand(0xD5);	//������ʾʱ�ӷ�Ƶ��/����Ƶ��
	OLED_WriteCommand(0x80);
	
	
#if OLED_SH1107_EN==1

	OLED_WriteCommand(0xA8);	//���ö�·������
	OLED_WriteCommand(0x7F);  //���ݸ߶ȵ�����ֵ
	OLED_WriteCommand(0x20);	//ҳ��ַģʽ
#else 

	OLED_WriteCommand(0xA8);	//���ö�·������
	OLED_WriteCommand(0x3F);  //���ݸ߶ȵ�����ֵ
	

#endif

	
	OLED_WriteCommand(0xD3);	//������ʾƫ��
	OLED_WriteCommand(0x00);
	
//	OLED_WriteCommand(0xDC);	//������ʾƫ��
//	OLED_WriteCommand(0x00);		
	OLED_WriteCommand(0x40);	//������ʾ��ʼ��


	
	OLED_WriteCommand(0xA1);	//�������ҷ���0xA1���� 0xA0���ҷ���
	
	OLED_WriteCommand(0xC8);	//�������·���0xC8���� 0xC0���·���

	OLED_WriteCommand(0xDA);	//����COM����Ӳ������
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//���öԱȶȿ���
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//����Ԥ�������
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//����VCOMHȡ��ѡ�񼶱�
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//����������ʾ��/�ر�

	OLED_WriteCommand(0xA6);	//��������/��ת��ʾ

	OLED_WriteCommand(0x8D);	//���ó���
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//������ʾ
		
	OLED_Clear();				//OLED����
}


