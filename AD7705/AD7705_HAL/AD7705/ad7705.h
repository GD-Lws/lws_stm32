#ifndef __AD7705_H
#define __AD7705_H	
#include "spi.h"

void AD7705_WriteByte(uint8_t Dst_Addr);
void Init_AD7705(uint8_t chnanel);
uint16_t GetData7705_CH1(void);
uint16_t GetData7705_CH2(void);
void ADC_7705(void);
void ADC_7705_double(void);

#endif 
