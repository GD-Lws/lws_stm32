#ifndef __MT6825_H
#define __MT6825_H	 
#include "sys.h" 

u32 get_Angle(void);
void bsp_spi_InitMT6825(void);
void MT6825_ConfigGPIO(void);

#endif
