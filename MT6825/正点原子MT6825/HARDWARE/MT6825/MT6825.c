#include "MT6825.h" 
#include "spi.h"
#include "delay.h"

/* CSNƬѡ */
#define RCC_CS 		RCC_AHB1Periph_GPIOB
#define PORT_CS		GPIOB
#define PIN_CS		GPIO_Pin_14

#define CS_0()						GPIO_ResetBits(PORT_CS, PIN_CS)
#define CS_1()						GPIO_SetBits(PORT_CS, PIN_CS)

static void MT6825_ConfigGPIO(void);

static u8 Spi_TxData[4]={0x83,0xff,0xff,0xff};///////03 04 05 寄存器存角度
static u8 Spi_pRxData[4]={0};

void bsp_spi_InitMT6825(void)
{
	MT6825_ConfigGPIO();	
	CS_1();
	SPI1_Init();		   			
	
}

static void MT6825_ConfigGPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;


	RCC_AHB1PeriphClockCmd(RCC_CS, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	// 上拉，下降沿触发
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	
	GPIO_InitStructure.GPIO_Pin = PIN_CS;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);
}

u32 get_Angle(void){
	u32 get_angle_18bit = 0;
	u8 i = 0;
	for (i = 0; i < 4; i++)
	{
		CS_0();
		delay_us(20);
		Spi_pRxData[i] = SPI1_ReadWriteByte(Spi_TxData[i]);
		delay_us(20);
		CS_1();
	}
	get_angle_18bit=((Spi_pRxData[1]<<10)|((Spi_pRxData[2]&0xFC)<<2)|(Spi_pRxData[3]>>4));////18bits 
	return(get_angle_18bit);
	
}


 














