#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "encode.h"
#include "adc.h"



int main(void)
{ 
	int en_count = 0;//����������
	int Realyangle = 0; 
	float get_v;
	u16 duty_ratio=0; 
	u8 t = 0;
	u16 adcx;
	u8 serial_len = 0;
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
 	TIM14_PWM_Init(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.   
	TIM4_Mode_Config();
	Adc_Init();
	
   while(1) //ʵ�ֱȽ�ֵ��0-300��������300���300-0�ݼ���ѭ��
	{
		en_count = TIM4->CNT;
		Realyangle = TIM_GetCounter(TIM4)/4/4096 ;
 		delay_ms(100);
		/*if(USART_RX_STA&0x8000)
		{
			serial_len = USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			if(serial_len != 0)
			{
				u16 receive_ratio = 0;
				for(t = 0; t < serial_len; t++)
				{
					if(USART_RX_BUF[t] - '0' >= 0)
					{receive_ratio = receive_ratio*10 + (USART_RX_BUF[t] - '0');}
				}
				duty_ratio = (receive_ratio * (-5)) + 500;
				printf("װ��ֵ%d \n",duty_ratio);
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
		TIM_SetCompare1(TIM14,duty_ratio);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	}
}
