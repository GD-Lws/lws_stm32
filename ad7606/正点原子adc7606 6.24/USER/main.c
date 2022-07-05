#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "spi.h"
#include "ADC76.h"
#include "key.h"  
static int16_t s_volt[8];
static int16_t s_dat[8];


/*
*********************************************************************************************************
*	函 数 名: AD7606_Mak
*	功能说明: 处理采样后的数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_Mak(void)
{
	uint8_t i;
	int16_t adc;

	for (i = 0;i < 8; i++)
	{	
		s_dat[i] = AD7606_ReadAdc(i);
	/* 
		32767 = 5V , 这是理论值，实际可以根据5V基准的实际值进行公式矫正 
		volt[i] = ((int16_t)dat[i] * 5000) / 32767;	计算实际电压值（近似估算的），如需准确，请进行校准            
		volt[i] = dat[i] * 0.3051850947599719
	*/
		
		adc = s_dat[i];
		if (g_tAD7606.Range == 0)
		{
			s_volt[i] = (adc * 5000) / 32767;
		}
		else
		{
			s_volt[i] = (adc * 10000) / 32767;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_Disp
*	功能说明: 显示采样后的数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_Disp(void)
{
	int16_t i;	
	int16_t iTemp;

	/* 打印采集数据 */
	for (i = 0; i < 8; i++)
	{                
   		iTemp = s_volt[i];	/* uV  */
		
		if (s_dat[i] < 0)
		{
			iTemp = -iTemp;
            printf(" CH%d = %6d,0x%04X (-%d.%d%d%d V) \r\n", i+1, s_dat[i], (uint16_t)s_dat[i], iTemp /1000, (iTemp%1000)/100, (iTemp%100)/10,iTemp%10);
		}
		else
		{
         	printf(" CH%d = %6d,0x%04X ( %d.%d%d%d V) \r\n", i+1, s_dat[i], s_dat[i] , iTemp /1000, (iTemp%1000)/100, (iTemp%100)/10,iTemp%10);                    
		}
	}
	printf("\33[%dA", (int)8);  /* 光标上移n行 */		
}

int main(void)
{ 
	u8 key;
	u16 i=0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED
	KEY_Init(); 				//按键初始化  
	
	bsp_spi_InitAD7606();

	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1按下
		{
			  printf("get_adc:/r/n");
				AD7606_Scan();
				AD7606_Mak();
				AD7606_Disp();
		}
		if(key==KEY0_PRES)//KEY0按下
		{
 			
		} 
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED0=!LED0;//提示系统正在运行	
			i=0;
		}		   
	}       
}

