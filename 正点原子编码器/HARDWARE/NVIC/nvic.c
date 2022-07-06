#include "nvic.h"
#include "misc.h"
#include "delay.h"

void PA_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure1)
{
  
	NVIC_InitStructure1.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure1);
}


//2----定时器的编码器接口模式,对编码器进行计数
void TIM4_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure2)
{
    NVIC_InitStructure2.NVIC_IRQChannel=TIM4_IRQn; 
    NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority=0x01; 
    NVIC_InitStructure2.NVIC_IRQChannelSubPriority=0x01; 
    NVIC_InitStructure2.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure2);

}


//3----串口1通信，用于Modbus-Rtu-485S
void USART1_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure4)
{
	NVIC_InitStructure4.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure4.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure4.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure4.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure4);
}


void NVIC_Config(u8 Interrupt_flag)
{

  	NVIC_InitTypeDef   NVIC_InitStructure;
	/* 配置中断使用组合  抢占式3位(0-7)，响应式1位(0-1) */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	if(Interrupt_flag == 1)
	{
		/*外部中断1，Z相归零----优先级最高 0 0*/
		PA_NVIC_Config(NVIC_InitStructure);
	}
	
	if(Interrupt_flag == 2)
	{	
		/*TIM4中断，编码器解码----优先级 1  1*/
		TIM4_NVIC_Config(NVIC_InitStructure);
	}
	if(Interrupt_flag == 3)
	{
		/* USART1中断，数据接收----优先级 2  1*/
		USART1_NVIC_Config(NVIC_InitStructure);
	}
}


