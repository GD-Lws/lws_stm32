#include "adc76.h"
#include "delay.h" 
#include "spi.h"

/* SPI���ߵ�SCK��MOSI��MISO �� bsp_spi_bus.c������  */
/* CSNƬѡ */
#define RCC_CS 		RCC_AHB1Periph_GPIOE
#define PORT_CS		GPIOE
#define PIN_CS		GPIO_Pin_11

/* RESET ���� */
#define RCC_RESET 	RCC_AHB1Periph_GPIOE
#define PORT_RESET	GPIOE
#define PIN_RESET	GPIO_Pin_13

/* RANGE ���̷�Χ��*/		
#define RCC_RANGE 	RCC_AHB1Periph_GPIOB
#define PORT_RANGE 	GPIOB
#define PIN_RANGE 	GPIO_Pin_10

#define RESET_0()	GPIO_ResetBits(PORT_RESET, PIN_RESET)
#define RESET_1()	GPIO_SetBits(PORT_RESET, PIN_RESET)

/* CONVST �����ź� */		
#define RCC_CONVST 	RCC_AHB1Periph_GPIOE
#define PORT_CONVST	GPIOE
#define PIN_CONVST 	GPIO_Pin_15

/* BUSY æµ�ź� */
#define RCC_BUSY 	RCC_AHB1Periph_GPIOE
#define PORT_BUSY 	GPIOE
#define PIN_BUSY 	GPIO_Pin_10

#define CH_NUM			8				/* �ɼ�2ͨ�� */

#define BUSY_IS_LOW()				(GPIO_ReadInputDataBit(PORT_BUSY, PIN_BUSY) == Bit_RESET)

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */


static void AD7606_ConfigGPIO(void);
void AD7606_Reset(void);	
void AD7606_SetInputRange(uint8_t _ucRange);
void AD7606_StartConv(void);
void AD7606_CfgSpiHard(void);

static int16_t s_adc_now[8];

AD7606_T g_tAD7606;


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitAD7606
*	����˵��: ��ʼ��AD7606 SPI����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_spi_InitAD7606(void)
{
	SPI1_Init();		   			//��ʼ��SPI
	
	AD7606_ConfigGPIO();		/* ����GPIO */
	
	AD7606_SetInputRange(g_tAD7606.Range);	/* ���ò������� */

	/* ���ù�����ģʽ */
	//ad7606_SetOS(0);
	AD7606_Reset();				/* Ӳ����λ��AD7606 */
	
	AD_CONVST_HIGH();			/* CONVST������Ϊ�ߵ�ƽ */	
}


/*
*********************************************************************************************************
*	�� �� ��: AD7606_ConfigGPIO
*	����˵��: ����GPIO�� ������ SCK  MOSI  MISO ������SPI���ߡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AD7606_ConfigGPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_CS | RCC_RANGE | RCC_BUSY | RCC_RESET | RCC_CONVST, ENABLE);

	/* ���ü����������IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* ���������費ʹ�� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = PIN_RESET;
	GPIO_Init(PORT_RESET, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_CONVST;
	GPIO_Init(PORT_CONVST, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_RANGE;
	GPIO_Init(PORT_RANGE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_CS;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);

	/* ����GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = PIN_BUSY;
	GPIO_Init(PORT_BUSY, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_SetInputRange
*	����˵��: ����AD7606ģ���ź��������̡�
*	��    ��: _ucRange : 0 ��ʾ����5V   1��ʾ����10V
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_SetInputRange(uint8_t _ucRange)
{
	if (_ucRange == 0)
	{
		AD_RANGE_5V();	/* ����Ϊ����5V */
	}
	else
	{
		AD_RANGE_10V();	/* ����Ϊ����10V */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_RESET
*	����˵��: Ӳ����λ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_Reset(void)
{
	/* CS  SCLK���� */
#ifdef SOFT_SPI
		AD_CS_1();
		//SCK_1();
#endif

 	/* AD7606�Ǹߵ�ƽ��λ��Ҫ����С����50ns */
	AD_RESET_LOW();
	
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	
	AD_RESET_LOW();
}


/*
*********************************************************************************************************
*	�� �� ��: ad7606_StartConv
*	����˵��: ����AD7606��ADCת��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_StartConv(void)
{
	/* �����ؿ�ʼת�����͵�ƽ����ʱ������25ns  */
	AD_CONVST_LOW();
	AD_CONVST_LOW();
	AD_CONVST_LOW();	/* ����ִ��2�Σ��͵�ƽԼ50ns */
	
	AD_CONVST_HIGH();
}


/*
*********************************************************************************************************
*	�� �� ��: AD7606_scan
*	����˵��: ɨ����ñ����������ڶ�ȡADת��������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_Scan(void) 		/* �˺������밴��ʱ���д */
{
	uint8_t i;			

	/* BUSY = 0 ʱ.ad7606���ڿ���״̬adת������ */	
	if (BUSY_IS_LOW())	   
    {
		AD_CS_0(); /* SPIƬѡ = 0 */
	
		for (i = 0; i < CH_NUM; i++)
		{
			s_adc_now[i] = bsp_spiRead1();
			
			s_adc_now[i] = s_adc_now[i] * 256 + bsp_spiRead1(); /* ������ */	
		}
	
		AD_CS_1(); /* SPIƬѡ = 1 */

		AD7606_StartConv();	/* ����ʼ�ź� */	
	}
}


/*
*********************************************************************************************************
*	�� �� ��: GetAdcFormFifo
*	����˵��: ��FIFO�ж�ȡһ��ADCֵ
*	��    �Σ�_usReadAdc : ���ADC����ı���ָ��
*	�� �� ֵ: 1 ��ʾOK��0��ʾ��������
*********************************************************************************************************
*/
int16_t AD7606_ReadAdc(uint8_t _ch)
{
	int16_t sAdc;
	
	DISABLE_INT();	
	sAdc = s_adc_now[_ch];
	ENABLE_INT();

	return sAdc;
}

 














