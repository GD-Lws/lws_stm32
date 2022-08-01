#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "CAN_MCP2515.h"
#include "key.h"  


const u8 TEXT_Buffer[]={"Explorer STM32F4 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)	 
	
int main(void)
{ 
	u8 key;
	u16 i=0;
	u8 can_init_flag = 0;
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
	KEY_Init(); 				//按键初始化  
	can_init_flag = MCP2515_Init();
	printf("can_iniit_flag: %d", can_init_flag);
	while(1)
	{
		Test_Mcp2515_Can();
	//	key=KEY_Scan(0);
	//	if(key==KEY1_PRES)//KEY1按下,写入W25Q128
	//	{
	//		printf("KEY_1_prs \r\n");			
	//	}
	//	if(key==KEY0_PRES)//KEY0按下,读取字符串并显示
	//	{
 		
	//} 
	//	i++;
	//	delay_ms(10);
	//	if(i==20)
	//	{
	//		LED0=!LED0;//提示系统正在运行	
	//		i=0;
	//	}	

	}       
}

