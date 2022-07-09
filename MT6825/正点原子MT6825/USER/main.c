#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "spi.h"
#include "MT6825.h"
#include "key.h"  

int main(void)
{ 
	u8 key;
	u16 i=0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);     
	uart_init(38400);	
	LED_Init();
	KEY_Init();
	
	bsp_spi_InitMT6825();

	while(1)
	{
		printf("get_angle:\r\n");
		u32 MT6825_data = get_Angle();
		printf("MT6825: %d", MT6825_data);
		delay_ms(500);
		key=KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1
		{
		
		}
		if(key==KEY0_PRES)//KEY0
		{
 			
		} 
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED0=!LED0;
			i=0;
		}		   
	}       
}

