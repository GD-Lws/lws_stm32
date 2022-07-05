#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "spi.h"
#include "ad7705.h"
#include "key.h"  


//ALIENTEK ̽����STM32F407������ ʵ��25
//SPIͨ�Žӿ�ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//�������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK
 
 
 
u8 num1[7];
u32 l_ncm1;
u8 num2[7];
u32 l_ncm2;
	
int main(void)
{ 
	u8 key;
  u8 i;
	u16 adcx;
	float temp;
	u16 dacval=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	KEY_Init();
	LED_Init();
	SPI1_Init();
	Init_AD7705(1);
	Init_AD7705(2);
	Dac1_Init();
	DAC_SetChannel1Data(DAC_Align_12b_R,dacval);//初始值为 0

	while(1)
	{
		
		ADC_7705();
		key=KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1����,д��W25Q128
		{
			printf("KEY1_PUSH \r\n");
			if(dacval<4000)dacval+=200;
			DAC_SetChannel1Data(DAC_Align_12b_R, dacval);//设置 DAC 
		
		}
		if(key==KEY0_PRES)//KEY0����,��ȡ�ַ�������ʾ
		{
 			printf("KEY0_PUSH");
		    if(dacval>200)dacval-=200;
			else dacval=0;
			DAC_SetChannel1Data(DAC_Align_12b_R, dacval);//设置 DAC 值
			

		}
		adcx=DAC_GetDataOutputValue(DAC_Channel_1);
		temp = (float)adcx*(3.3/4096); 
		printf("temp_v: %lf V\r\n", temp);
		i++;
		delay_ms(50);
		if(i==20)
		{
			LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}		   
	}       
}

