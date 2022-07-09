#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
		  
void SPI1_Init(void);			 //��ʼ��SPI1��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
uint8_t bsp_spiRead1(void);
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�
		 
#endif

