#include "ad7705.h"
#include "delay.h"	   
#include "usart.h"	

#define CS_ADC_LOW()    GPIO_ResetBits(GPIOA,GPIO_Pin_5)   
#define CS_ADC_HIGH()   GPIO_SetBits(GPIOA,GPIO_Pin_5)

void AD7705_IO_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_APB2Periph_SPI1|RCC_AHB1Periph_GPIOA, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;// AdDrdy
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ;//PC4配置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);               
	
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;         //SPI CS   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);   
}

//写数据
void AD7705_WriteByte(u8 Dst_Addr)   
{   
        CS_ADC_LOW();//使能器件  
        delay_us(20);
        SPI1_ReadWriteByte(Dst_Addr);
        delay_us(100);
        CS_ADC_HIGH();//使能器件  
}


/********AD7705初始化函数***********/

void Init_AD7705(u8 chnanel)
{
	AD7705_IO_Init();
	CS_ADC_LOW();       
    SPI1_ReadWriteByte(0xff);//启动传输                 
  	CS_ADC_HIGH();
    u8 i;
    for(i=0;i<150;i++)/* 多于连续32个 DIN=1 使串口复位 */
    {
        AD7705_WriteByte(0xff);//持续DIN高电平写操作，恢复AD7705接口
    }  
    delay_ms(1);
    switch(chnanel)
    {
      case 1:
             AD7705_WriteByte(0x20); /* 写时钟寄存器选中ch1*/
             AD7705_WriteByte(0x0f); /* 4.9152MHz时钟，250Hz数据更新速率 */
             AD7705_WriteByte(0x10); /*选择设置寄存器,使用chnanel 1*/
             AD7705_WriteByte(0x4c); //写设置寄存器 ,设置成双极性、无缓冲、增益为2、滤波器工作、自校准
      break;
			/*有更改，时钟寄存器设为0x0a，4.9152MHz时钟，500Hz数据更新速率，*/
      case 2:
             AD7705_WriteByte(0x21); /* 写时钟寄存器选中ch2 */
             AD7705_WriteByte(0x0f); /* 4.9152MHz时钟，500Hz数据更新速率 */
             AD7705_WriteByte(0x11); /*选择设置寄存器,使用chnane 2*/
             AD7705_WriteByte(0x4c); //写设置寄存器,设置成双极性、无缓冲、增益为2、滤波器工作、自校准
       break;
       default:       
                break;
        }
}

/* 读AD7705转换数据 输入通道channel */
u16 GetData7705_CH1(void)
{
    u16 temp1 = 0;
    u16 DataL = 0;
    u16 DataH = 0;
    Init_AD7705(1);                        //初始化通道1
    delay_ms(1);
    AD7705_WriteByte(0x39);   //选中CH1数据寄存器读  
    while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4==1)){}//待数据准备好AdDrdy=0  
    CS_ADC_LOW(); //使能器件  
    delay_us(20);
    DataH = SPI1_ReadWriteByte(0xff);
    DataL = SPI1_ReadWriteByte(0xff);
    delay_us(100);
    CS_ADC_HIGH();   //取消片选        
    DataH = DataH << 8;
    temp1  =  DataH | DataL;

    return temp1;
}


/* 读AD7705转换数据 输入通道channel */
u16 GetData7705_CH2(void)
{
    u16 temp2 = 0;
    u16 DataL = 0;
    u16 DataH = 0;
    Init_AD7705(2);                //初始化通道2
    delay_ms(1);
    AD7705_WriteByte(0x38);   //选中CH2数据寄存器读  
    while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4==1));  //待数据准备好AdDrdy=0  
    CS_ADC_LOW(); //使能器件  
    delay_us(20);
    DataH = SPI1_ReadWriteByte(0xff);
    DataL = SPI1_ReadWriteByte(0xff);
  	delay_us(100);
    CS_ADC_HIGH();   //取消片选        
    DataH = DataH << 8;
    temp2  =  DataH | DataL;

    return temp2;
}  


//数据处理

extern u8 num1[];
extern u32 l_ncm1;
extern u8 num2[];
extern u32 l_ncm2;
void ADC_7705(void)
{        
  u8 i = 0;
	u32 l_ncm1_25 = 0;
	u32 l_ncm1_33 = 0;
	u32 l_ncm1_50 = 0;
	u16 RCH1_16bit,RCH2_16bit;     
	
	RCH1_16bit = GetData7705_CH1();								
	l_ncm1  = (u32)(RCH1_16bit*(122500.0/65535)); //算出通道1电压

	
	RCH2_16bit = GetData7705_CH2();
	l_ncm2  = (u32)(RCH2_16bit*(122500.0/65535)); //算出通道2电压

	num1[0]    = l_ncm1/10000+'0';
	num1[2]    = (l_ncm1%10000)/1000+'0';
	num1[3]    = (l_ncm1%1000)/100+'0';
	num1[4]    = (l_ncm1%100)/10+'0';
	num1[5]    = l_ncm1%10+'0';

	num2[0]    = l_ncm2/10000+'0';
	num2[2]    = (l_ncm2%10000)/1000+'0';
	num2[3]    = (l_ncm2%1000)/100+'0';
	num2[4]    = (l_ncm2%100)/10+'0';
	num2[5]    = l_ncm2%10+'0';
	printf("l_ncm1: %d", l_ncm1);
	printf("\r\n");
	printf("l_ncm_2 %d \r \n", l_ncm2);
	
	if(l_ncm2>8500|l_ncm2<8200)
	{
		delay_ms(10);
		l_ncm2=0;		
	}
	else
	{
		printf("l_ncm_1 %d", l_ncm1);
	}
      
}


















