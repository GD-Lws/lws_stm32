#ifndef __AD7705_H
#define __AD7705_H	
#include "spi.h"

void AD7705_WriteByte(u8 Dst_Addr);
void Init_AD7705(u8 chnanel);
u16 GetData7705_CH1(void);
u16 GetData7705_CH2(void);
void ADC_7705(void);
void ADC_7705_double(void);

#endif 
