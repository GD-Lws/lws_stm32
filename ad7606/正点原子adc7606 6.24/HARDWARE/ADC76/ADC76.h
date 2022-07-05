#ifndef __ADC76_H
#define __ADC76_H	 
#include "sys.h" 

/* 片选 */
#define AD_CS_0()						GPIO_ResetBits(PORT_CS, PIN_CS)
#define AD_CS_1()						GPIO_SetBits(PORT_CS, PIN_CS)

/* 设置量程 */
#define AD_RANGE_5V()					GPIO_ResetBits(PORT_RANGE, PIN_RANGE)
#define AD_RANGE_10V()					GPIO_SetBits(PORT_RANGE, PIN_RANGE)	//置1

/* 复位引脚 */
#define AD_RESET_LOW()					GPIO_ResetBits(PORT_RESET, PIN_RESET)
#define AD_RESET_HIGH()					GPIO_SetBits(PORT_RESET, PIN_RESET)

/* 起始信号 */
#define	AD_CONVST_LOW()					GPIO_ResetBits(PORT_CONVST, PIN_CONVST)
#define	AD_CONVST_HIGH()				GPIO_SetBits(PORT_CONVST, PIN_CONVST)

typedef struct
{
	uint8_t Range;
}AD7606_T;

void AD7606_Reset(void);		
void AD7606_SetInputRange(uint8_t _ucRange);	/* 设置AD7606量程 */
void bsp_spi_InitAD7606(void);		/* 初始化AD7606 */
void AD7606_Scan(void); 		/* 此函数代码按照时序编写 */
int16_t AD7606_ReadAdc(uint8_t _ch);
void AD7606_SetOS(uint8_t _ucMode);

extern AD7606_T g_tAD7606;


#endif
