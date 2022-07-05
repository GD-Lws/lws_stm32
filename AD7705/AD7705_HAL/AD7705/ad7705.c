#include "ad7705.h"	   
#include "usart.h"	
#include "spi.h"
#include "main.h"


#define CS_ADC_LOW()    digitalLo(AD7705_CS_GPIO_Port,AD7705_CS_Pin)   
#define CS_ADC_HIGH()   digitalHi(AD7705_CS_GPIO_Port,AD7705_CS_Pin)   


//写数据
void AD7705_WriteByte(uint8_t Dst_Addr)   
{   
		CS_ADC_LOW();//使能器件  
		SPI1_ReadWriteByte(Dst_Addr);
		CS_ADC_HIGH();//使能器件  
}


/********AD7705初始化函数***********/

void Init_AD7705(uint8_t chnanel)
{
    CS_ADC_LOW();      
    //启动传输   
    SPI1_ReadWriteByte(0xff);              
  	CS_ADC_HIGH();
	  
    uint8_t i;
    for(i=0;i<150;i++)/* 多于连续32个 DIN=1 使串口复位 */
    {
        AD7705_WriteByte(0xff);//持续DIN高电平写操作，恢复AD7705接口
    }  
		//printf("CS_ENABLE");
    HAL_Delay(1);
    switch(chnanel)
    {
        case 1:
              AD7705_WriteByte(0x20); /* 写时钟寄存器选中ch1*/
              AD7705_WriteByte(0x0f); /* 4.9152MHz时钟，500Hz数据更新速率 */
              AD7705_WriteByte(0x10); /*选择设置寄存器,使用chnanel 1*/
              AD7705_WriteByte(0x4c); //写设置寄存器 ,设置成双极性、无缓冲、增益为2、滤波器工作、自校准
							printf("Channel_1_init\r\n");
        break;
        /*有更改，时钟寄存器设为0x0a，4.9152MHz时钟，500Hz数据更新速率，*/
        case 2:
              AD7705_WriteByte(0x21); /* 写时钟寄存器选中ch2 */
              AD7705_WriteByte(0x0f); /* 4.9152MHz时钟，500Hz数据更新速率 */
              AD7705_WriteByte(0x11); /*选择设置寄存器,使用chnane 2*/
              AD7705_WriteByte(0x4c); //写设置寄存器,设置成双极性、无缓冲、增益为2、滤波器工作、自校准
							printf("Channel_2_init\r\n");
        break;
        default:       
        break;
    }
}

/* 读AD7705转换数据 输入通道channel */
uint16_t GetData7705_CH1(void)
{
    uint16_t temp1 = 0;
    uint16_t DataL = 0;
    uint16_t DataH = 0;
    Init_AD7705(1);                        //初始化通道1
    HAL_Delay(1);
    AD7705_WriteByte(0x39);   //选中CH1数据寄存器读  
    while(HAL_GPIO_ReadPin(AD7705_ADDR_GPIO_Port, AD7705_ADDR_Pin) == 0){}//待数据准备好AdDrdy=0  
    CS_ADC_LOW(); //使能器件  
    
    DataH = SPI1_ReadWriteByte(0xff);
    DataL = SPI1_ReadWriteByte(0xff);
    
    CS_ADC_HIGH();   //取消片选        
    DataH = DataH << 8;
    temp1  =  DataH | DataL;

    return temp1;
}


/* 读AD7705转换数据 输入通道channel */
uint16_t GetData7705_CH2(void)
{
    uint16_t temp2 = 0;
    uint16_t DataL = 0;
    uint16_t DataH = 0;
    Init_AD7705(2);                //初始化通道2
    HAL_Delay(1);
    AD7705_WriteByte(0x38);   //选中CH2数据寄存器读  
    while(HAL_GPIO_ReadPin(AD7705_ADDR_GPIO_Port, AD7705_ADDR_Pin) == 0);  //待数据准备好AdDrdy=0  
    CS_ADC_LOW(); //使能器件  
    // DataH = SPI1_ReadWriteByte(0xff);
    // DataL = SPI1_ReadWriteByte(0xff);
    DataH = SPI1_ReadWriteByte(0xff);
    DataL = SPI1_ReadWriteByte(0xff);
    CS_ADC_HIGH();   //取消片选        
    DataH = DataH << 8;
    temp2  =  DataH | DataL;
    
    return temp2;
}  


//数据处理
extern uint8_t num1[];
extern uint32_t l_ncm1;
extern uint8_t num2[];
extern uint32_t l_ncm2;
void ADC_7705(void)
{        
	uint16_t RCH1_16bit,RCH2_16bit;     
	RCH1_16bit = GetData7705_CH1();								
	l_ncm1  = (uint32_t)(RCH1_16bit*(122500.0/65535)); //算出通道1电压

	
	RCH2_16bit = GetData7705_CH2();
	l_ncm2  = (uint32_t)(RCH2_16bit*(122500.0/65535)); //算出通道2电压

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
	printf("l_ncm_1: %d \r\n", l_ncm1);
	printf("l_ncm_2: %d \r\n", l_ncm2);
	
	if(l_ncm2>8500|l_ncm2<8200)
	{
		HAL_Delay(10);
		l_ncm2=0;		
	}
	else
	{
		printf("l_ncm_1 %d", l_ncm1);
	}
}


















