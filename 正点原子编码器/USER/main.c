#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "encode.h"
#include "adc.h"



int main(void)
{ 
	int en_count = 0;//编码器计数
	int Realyangle = 0; 
	float get_v;
	u16 duty_ratio=0; 
	u8 t = 0;
	u16 adcx;
	u8 serial_len = 0;
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200
 	TIM14_PWM_Init(500-1,84-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.   
	TIM4_Mode_Config();
	Adc_Init();
	
   while(1) //实现比较值从0-300递增，到300后从300-0递减，循环
	{
		en_count = TIM4->CNT;
		Realyangle = TIM_GetCounter(TIM4)/4/4096 ;
 		delay_ms(100);
		/*if(USART_RX_STA&0x8000)
		{
			serial_len = USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			if(serial_len != 0)
			{
				u16 receive_ratio = 0;
				for(t = 0; t < serial_len; t++)
				{
					if(USART_RX_BUF[t] - '0' >= 0)
					{receive_ratio = receive_ratio*10 + (USART_RX_BUF[t] - '0');}
				}
				duty_ratio = (receive_ratio * (-5)) + 500;
				printf("装入值%d \n",duty_ratio);
				USART_RX_STA=0;
			}
			serial_len=0;
		}*/
		adcx = Get_Adc_Average(ADC_Channel_5,20);
	  get_v = (float)adcx*(3.3/4096);
		printf("count: %d \r\n",en_count);
		printf("Realyangle: %d \r\n",Realyangle);
		printf("circle_count: %d \r\n",circle_count);
		printf("Get_v: %f\r\n",get_v);
		printf("\r\n");
		TIM_SetCompare1(TIM14,duty_ratio);	//修改比较值，修改占空比
	}
}
