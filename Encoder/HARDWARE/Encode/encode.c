#include "encode.h" 
#include "zexit.h"
#include "nvic.h"

void TIM4_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure; 
	
	//PB6¡¢7Ê¹ÄÜ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//Ê¹ÄÜTIM4Ê±ÖÓ  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//B7 B6
	GPIO_StructInit(&GPIO_InitStructure);//½«GPIO_InitStructÖÐµÄ²ÎÊý°´È±Ê¡ÖµÊäÈë
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//PB6 PB7¸¡¿ÕÊäÈë  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //PA6/7¸´ÓÃÎ»¶¨Ê±Æ÷4
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
	
	NVIC_Config(2);	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = (4096*4 - 1) ;  //Éè¶¨¼ÆÊýÆ÷ÖØ×°Öµ   TIMx_ARR ÉèÖÃÏßÊý (1000*4)-1
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM4Ê±ÖÓÔ¤·ÖÆµÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//ÉèÖÃÊ±ÖÓ·Ö¸î T_dts = T_ck_int    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIMÏòÉÏ¼ÆÊý 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//Ê¹ÓÃ±àÂëÆ÷Ä£Ê½3£¬ÉÏÉýÏÂ½µ¶¼¼ÆÊý£»¶þÄ±¶Æµ
  TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 2;  //Ñ¡ÔñÊäÈë±È½ÏÂË²¨Æ÷ 
	TIM_ICInit(TIM4, &TIM_ICInitStructure);//½«TIM_ICInitStructureÖÐµÄÖ¸¶¨²ÎÊý³õÊ¼»¯TIM4
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//ÔÊÐíTIM4Òç³öÖÐ¶Ï
	
	//Reset counter
  TIM_SetCounter(TIM4,0); //TIM4->CNT=0
  TIM_Cmd(TIM4, ENABLE); //Ê¹ÄÜ
}

int circle_count=0;//È¦Êý
  
void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
    {     
        if((TIM4->CR1>>4 & 0x01)==0) //DIR==0
            circle_count++;
        else if((TIM4->CR1>>4 & 0x01)==1)//DIR==1
            circle_count--;
    }
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update); 
}








