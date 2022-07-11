#include "can_mcp2515.h"
#include "delay.h"
#include "stdio.h"

/* CAN 波特率选择 */
static u16 s_Baud = 125;
// static u16 s_Baud = 250; 
/* static u16 s_Baud = 500; */

/* MCP2515 外部晶振为 8M */
// static u32 s_Osc = 8000;
static u32 s_Osc = 16000;

/* mcp2515 控制指令 */
typedef enum
{
    RESET_CMD = 0xc0,           
    READ_CMD = 0x03,
    READ_RX_CMD	= 0x90,
    WRITE_CMD = 0x02,
    LOAD_TX_CMD = 0x40,
    RTS_CMD = 0x80,
    RTS_BUFF0_CMD = 0x81,
    RTS_BUFF1_CMD = 0x82,
    RTS_BUFF2_CMD = 0x84,
    READ_STATUS_CMD	= 0xA0,
    RX_STATUS_CMD = 0xB0,
    BIT_MODIFY_CMD = 0x05,
}_Cmd;

/* mcp2515 寄存器地址 */
typedef enum
{
    REG_RXF0SIDH = 0x00,
    REG_RXF0SIDL = 0x01,
    REG_RXF0EID8 = 0x02,
    REG_RXF0EID0 = 0x03,
    REG_RXF1SIDH = 0x04,
    REG_RXF1SIDL = 0x05,
    REG_RXF1EID8 = 0x06,
    REG_RXF1EID0 = 0x07,
    REG_RXF2SIDH = 0x08,
    REG_RXF2SIDL = 0x09,
    REG_RXF2EID8 = 0x0a,
    REG_RXF2EID0 = 0x0b,
    REG_BFPCTRL = 0x0c,
    REG_TXRTSCTRL = 0x0d,
    REG_CANSTAT = 0x0e,
    REG_CANCTRL = 0x0f,

    REG_RXF3SIDH = 0x10,
    REG_RXF3SIDL = 0x11,
    REG_RXF3EID8 = 0x12,
    REG_RXF3EID0 = 0x13,
    REG_RXF4SIDH = 0x14,
    REG_RXF4SIDL = 0x15,
    REG_RXF4EID8 = 0x16,
    REG_RXF4EID0 = 0x17,
    REG_RXF5SIDH = 0x18,
    REG_RXF5SIDL = 0x19,
    REG_RXF5EID8 = 0x1a,
    REG_RXF5EID0 = 0x1b,
    REG_TEC = 0x1c,
    REG_REC = 0x1d,

    REG_RXM0SIDH = 0x20,
    REG_RXM0SIDL = 0x21,
    REG_RXM0EID8 = 0x22,
    REG_RXM0EID0 = 0x23,
    REG_RXM1SIDH = 0x24,
    REG_RXM1SIDL = 0x25,
    REG_RXM1EID8 = 0x26,
    REG_RXM1EID0 = 0x27,
    REG_CNF3 = 0x28,
    REG_CNF2 = 0x29,
    REG_CNF1 = 0x2a,
    REG_CANINTE = 0x2b,
    REG_CANINTF = 0x2c,
    REG_EFLG = 0x2d,

    REG_TXB0CTRL = 0x30,
    REG_TXB0SIDH = 0x31,
    REG_TXB0SIDL = 0x32,
    REG_TXB0EID8 = 0x33,
    REG_TXB0EID0 = 0x34,
    REG_TXB0DLC = 0x35,
    REG_TXB0D0 = 0x36,
    REG_TXB0D1 = 0x37,
    REG_TXB0D2 = 0x38,
    REG_TXB0D3 = 0x39,
    REG_TXB0D4 = 0x3a,
    REG_TXB0D5 = 0x3b,
    REG_TXB0D6 = 0x3c,
    REG_TXB0D7 = 0x3d,

    REG_TXB1CTRL = 0x40,
    REG_TXB1SIDH = 0x41,
    REG_TXB1SIDL = 0x42,
    REG_TXB1EID8 = 0x43,
    REG_TXB1EID0 = 0x44,
    REG_TXB1DLC = 0x45,
    REG_TXB1D0 = 0x46,
    REG_TXB1D1 = 0x47,
    REG_TXB1D2 = 0x48,
    REG_TXB1D3 = 0x49,
    REG_TXB1D4 = 0x4a,
    REG_TXB1D5 = 0x4b,
    REG_TXB1D6 = 0x4c,
    REG_TXB1D7 = 0x4d,

    REG_TXB2CTRL = 0x50,
    REG_TXB2SIDH = 0x51,
    REG_TXB2SIDL = 0x52,
    REG_TXB2EID8 = 0x53,
    REG_TXB2EID0 = 0x54,
    REG_TXB2DLC = 0x55,
    REG_TXB2D0 = 0x56,
    REG_TXB2D1 = 0x57,
    REG_TXB2D2 = 0x58,
    REG_TXB2D3 = 0x59,
    REG_TXB2D4 = 0x5a,
    REG_TXB2D5 = 0x5b,
    REG_TXB2D6 = 0x5c,
    REG_TXB2D7 = 0x5d,

    REG_RXB0CTRL = 0x60,
    REG_RXB0SIDH = 0x61,
    REG_RXB0SIDL = 0x62,
    REG_RXB0EID8 = 0x63,
    REG_RXB0EID0 = 0x64,
    REG_RXB0DLC = 0x65,
    REG_RXB0D0 = 0x66,
    REG_RXB0D1 = 0x67,
    REG_RXB0D2 = 0x68,
    REG_RXB0D3 = 0x69,
    REG_RXB0D4 = 0x6a,
    REG_RXB0D5 = 0x6b,
    REG_RXB0D6 = 0x6c,
    REG_RXB0D7 = 0x6d,

    REG_RXB1CTRL = 0x70,
    REG_RXB1SIDH = 0x71,
    REG_RXB1SIDL = 0x72,
    REG_RXB1EID8 = 0x73,
    REG_RXB1EID0 = 0x74,
    REG_RXB1DLC = 0x75,
    REG_RXB1D0 = 0x76,
    REG_RXB1D1 = 0x77,
    REG_RXB1D2 = 0x78,
    REG_RXB1D3 = 0x79,
    REG_RXB1D4 = 0x7a,
    REG_RXB1D5 = 0x7b,
    REG_RXB1D6 = 0x7c,
    REG_RXB1D7 = 0x7d,
}_RegAddr;

/* 寄存器 位 */
typedef enum
{
    REG_BIT0 = (1<<0),
    REG_BIT1 = (1<<1),
    REG_BIT2 = (1<<2),
    REG_BIT3 = (1<<3),
    REG_BIT4 = (1<<4),
    REG_BIT5 = (1<<5),
    REG_BIT6 = (1<<6),
    REG_BIT7 = (1<<7),
    
    BIT_SET = 1,
    BIT_RESET = 0,
}_Bit;


static u8 Spi1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable the SPI clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    /* Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);

    /* SPI GPIO Configuration --------------------------------------------------*/
    /* Connect SPI pins to AF5 */  
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    /* SPI  SCK/MISO/MOSI  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI  CS */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI configuration -------------------------------------------------------*/
//    SPI_I2S_DeInit(SPI1);
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1,&SPI_InitStructure);

    /* Configure the SPI interrupt priority */
    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    SPI_Cmd(SPI1,ENABLE);
    
    return 1;
}

/* MCP2515 CS 片选 设置 */
static void MCP2515_CS(u8 status)
{
    if( status )
        GPIO_SetBits(GPIOA,GPIO_Pin_4);
    else
        GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}

/*****************************************************
function    : static u8 Spi_SendRecData(u8 data)
description : SPi ����8 λ���� ͬʱ���� 8 λ����
input       : data - �������� 
return      : fasle - ʧ��
              true  - �ɹ�
******************************************************/
static u8 Spi_SendRecData(u8 data)
{
    u32 i =0; 
    while ( (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) &&(i++<0xffff) );
    SPI_I2S_SendData(SPI1, data);
    
    i =0;
    while ( (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) &&(i++<0xffff) );
  
    return SPI_I2S_ReceiveData(SPI1);  
}

/*******************************************************
function    : static bool MCP2515_WriteReg(u8 reg,u8 data)
description : дmcp2515 �Ĵ��� 
input       : reg - �Ĵ�����ַ
              data - �Ĵ�������
return      : true  - �ɹ�
********************************************************/
static u8 MCP2515_WriteReg(u8 reg,u8 data)
{
    
    MCP2515_CS(0);
    
    Spi_SendRecData(WRITE_CMD);
    
    Spi_SendRecData(reg);
    
    Spi_SendRecData(data);
    
    MCP2515_CS(1);
    
    return 1;
}

/*******************************************************
function    : static void MCP2515_Load_TX_Buff(u8 reg)
description : дmcp2515 TX �Ĵ��� ID/data
input       : reg - �Ĵ�����ַ
              data - �Ĵ�������
return      : true  - �ɹ�
********************************************************/
static void MCP2515_Load_TX_Buff(u8 reg)
{
    MCP2515_CS(0);
    
    Spi_SendRecData(LOAD_TX_CMD);
       
    Spi_SendRecData(0xff);
    
    MCP2515_CS(1);

}

/*****************************************************
function    : static bool MCP2515_RTS(u8 BuffNum)
description : ������ 
input       : BuffNum - ������BUFF ��� (0-2)
return      : void
*******************************************************/
static void MCP2515_RTS(u8 BuffNum)
{
    MCP2515_CS(0);
    
    if( 0 == BuffNum)
        Spi_SendRecData(RTS_BUFF0_CMD);
    else if( 1 == BuffNum)
        Spi_SendRecData(RTS_BUFF1_CMD);
    else
        Spi_SendRecData(RTS_BUFF2_CMD);
    
    MCP2515_CS(1);
    
}

/*******************************************************
function    : static u8 MCP2515_ReadReg(u8 reg)
description : ��mcp2515 �Ĵ��� 
input       : reg - �Ĵ�����ַ
return      : ���ؼĴ�������
********************************************************/
static u8 MCP2515_ReadReg(u8 reg)
{
    u8 ret = 0;
    
    MCP2515_CS(0);
    
    Spi_SendRecData(READ_CMD);
   
    Spi_SendRecData(reg);
    
    ret = Spi_SendRecData(0xff);    /* 0xff�޹����� spi ʱ��ʹ�� */
    
    MCP2515_CS(1);
    
    return ret;
}


/*******************************************************
function    : static u8 MCP2515_ReadStaReg(void)
description : ��mcp2515 ״̬�Ĵ��� 
input       : reg - �Ĵ�����ַ
return      : ���ؼĴ�������
********************************************************/
static u8 MCP2515_ReadStaReg(void)
{
    u8 ret = 0;
    
    MCP2515_CS(0);
    
    Spi_SendRecData(READ_STATUS_CMD); 
    
    ret = Spi_SendRecData(0xff);    /* 0xff�޹����� spi ʱ��ʹ�� */
    
    MCP2515_CS(1);
    
    return ret;
}

/*****************************************************************************
function    : static bool MCP2515_BitModify(u8 reg,u8 BitSel,u8 BitSta)
description : �Ĵ��� λ�޸� 
input       : reg - �Ĵ�����ַ
              BitSel - ѡ���޸�λ
              BitSta - SET_BIT ��λ
                     - RESET_BIT ����
return      : true - �ɹ�
******************************************************************************/
static u8 MCP2515_BitModify(u8 reg,u8 BitSel,u8 BitSta)
{ 
    MCP2515_CS(0);
    
    Spi_SendRecData(BIT_MODIFY_CMD);
    
    Spi_SendRecData(reg);
    
    Spi_SendRecData(BitSel);    /* mask */
    
    if(BitSta)                  /* data */
        Spi_SendRecData(BitSel);
    else
        Spi_SendRecData(~BitSel);
    
    MCP2515_CS(1);
    
    return 1;
}

/*************************************************************************
function    :static bool MCP2515_WriteReg_Seq(u8 reg,u8 *data,u8 length)
description :д�Ĵ�������
input       : reg - �Ĵ�����ַ
              *data - ��������
              length - ���г��� 
return      : true - �ɹ�
***************************************************************************/
static u8 MCP2515_WriteReg_Seq(u8 reg,u8 *data,u8 length)
{
    u8 i=0;
    if( NULL == data )return 0;
    MCP2515_CS(0);
    
    Spi_SendRecData(WRITE_CMD);
    
    Spi_SendRecData(reg);
    
    for(i=0;i<length;i++)
        Spi_SendRecData(*data++);
    
    MCP2515_CS(1);
    
    return 1;
}


/*************************************************************************
function    :static bool MCP2515_ReadReg_Seq(u8 reg,u8 *data,u8 length)
description : ���Ĵ�������
input       : reg - �Ĵ�����ַ
              *data - �������ָ��
              length - ���г��� 
return      : true - �ɹ�
***************************************************************************/
static u8 MCP2515_ReadReg_Seq(u8 reg,u8 *data,u8 length)
{
    u8 i=0;
    if( NULL == data) return 0;
    MCP2515_CS(0);
    
    Spi_SendRecData(READ_CMD);
    
    Spi_SendRecData(reg);
    
    for(i=0;i<length;i++)
        *data++ = Spi_SendRecData(0xff);
    
    MCP2515_CS(1);
    
    return 1;
}

/*****************************************************
function    : static bool MCP2515_Reset(void)
description : ��λ MCP2515
input       : void
return      : true - �ɹ�
*******************************************************/
static u8 MCP2515_Reset(void)
{
    MCP2515_CS(0);
    
    Spi_SendRecData(RESET_CMD);
    
    MCP2515_CS(1);
    
    return 1;
}


/******************************************************
function    : static void MCP2515_Can_ConfigBaud(void)
description : ����CAN ͨ�Ų����� 
input       : void
return      : void 
********************************************************/
static void MCP2515_Can_ConfigBaud(void)
{
   /* - - - - - - - --- - - - - - - -- - - - - - - - - - 
    f = 1/t 
    t = T(syscseg) + T(proseg) + T(ps1) + T(ps2)
     
    T(syscseg)  : 1Tq           : SJW            
    T(proseg)   : 1Tq - 8Tq     : 
    T(ps1)      : 1Tq - 8Tq
    T(ps2)      : 2Tq - 8Tq
    
    Tq = (2*(BRP+1))/Fosc
    
    Init_Value: (f = 125k )
       T(syscseg) = 1 * Tq;
       T(proseg)  = 2 * Tq;
       T(ps1)     = 3 * Tq;
       T(ps2)     = 2 * Tq;
    - - - - - - -  - -  - -  - -  - -  - - - - - - - - - -*/
    u8 cnf1=0,cnf2=0,cnf3=0;
    
    cnf1 = s_Osc/s_Baud/16 - 1 ;        /* SJW : 00  */
    cnf2 = 0x91;                        /* Tpro = (PRSEG+1)Tq  Tps1 =(PHSEG1+1)Tq  */
    cnf3 = 0x01;                        /* Tps2 = (PHSEG2+1)Tq */
    
    /* send message  config */
    MCP2515_WriteReg(REG_CNF1,cnf1);
    MCP2515_WriteReg(REG_CNF2,cnf2);
    MCP2515_WriteReg(REG_CNF3,cnf3);
         
    /* receive message config */ 
    MCP2515_WriteReg(REG_RXB0CTRL,0x60);    /* ������������ MASK��Filter �� ���� */
    MCP2515_WriteReg(REG_RXM0SIDH,0x00);    /* MASK 00 �������б��� */
    MCP2515_WriteReg(REG_RXM0SIDL,0x00);
    MCP2515_WriteReg(REG_RXM0EID8,0x00);
    MCP2515_WriteReg(REG_RXM0EID0,0x00);
}

/**********************************************************************
function    : static void MCP2515_Can_ConfigMsg(_MCP2515_CAN_MSG *msg)
description : ���ñ�����Ϣ
input       : *msg - ������Ϣ
return      : void 
***********************************************************************/
static void MCP2515_Can_ConfigMsg(_MCP2515_CAN_MSG *msg)
{
    u8 sidh =0,sidl=0,eid8=0,eid0=0,dlc=0;
    
    if( msg->IDE )              /* ��չ֡ */
    {
        sidl = (u8)(msg->ExtId>>16) | 0x08;
        eid8 = (u8)(msg->ExtId>>8);
        eid0 = (u8)(msg->ExtId);
    }
    else                        /* ����֡ */
    {
        sidl = (u8)(msg->StdId<<5);
        sidh = (u8)(msg->StdId>>3);
    }
    
    if( msg->RTR )              /* Զ��֡ */
        dlc = msg->DLC | 0x40;
    else                        /* ����֡ */
        dlc = msg->DLC;
    
    MCP2515_BitModify(REG_TXB0CTRL,REG_BIT1|REG_BIT0,BIT_SET);        /* ����BUFF0 Ϊ��߷������ȼ� */
    MCP2515_WriteReg(REG_TXB1CTRL,0x02);     /* ����BUFF1Ϊ �����ȼ� */
    MCP2515_WriteReg(REG_TXB2CTRL,0x00);     /* ����BUFF2Ϊ ������ȼ� */
    
    MCP2515_WriteReg(REG_TXB0SIDH,sidh);
    MCP2515_WriteReg(REG_TXB0SIDL,sidl);
    MCP2515_WriteReg(REG_TXB0EID8,eid8);
    MCP2515_WriteReg(REG_TXB0EID0,eid0);
    MCP2515_WriteReg(REG_TXB0DLC,dlc);
    MCP2515_WriteReg_Seq(REG_TXB0D0,msg->Data,msg->DLC);    /* �������� */
}


/*******************************************************************
function    : static void MCP2515_Can_GetMsg(_MCP2515_CAN_MSG *msg)
description : ��ȡ���ձ�������
input       : ���뱨�ĵ� ���ݽṹ _MCP2515_CAN_MSG ��
return      : void
*********************************************************************/
static void MCP2515_Can_GetMsg(_MCP2515_CAN_MSG *msg)
{
    u32 sidh =0,sidl=0,eid8=0,eid0=0,dlc=0;
    
    if( NULL == msg) return;
    
    sidh = MCP2515_ReadReg(REG_RXB0SIDH);
    sidl = MCP2515_ReadReg(REG_RXB0SIDL);
    eid8 = MCP2515_ReadReg(REG_RXB0EID8);
    eid0 = MCP2515_ReadReg(REG_RXB0EID0);
    dlc = MCP2515_ReadReg(REG_RXB0DLC);

    msg->IDE = (sidl>>3)&0x0001;      /* ��׼֡ or ��չ֡ */
    msg->RTR = (sidl>>4)&0x0001;      /* ����֡ or Զ��֡ */
    
    if( msg->IDE )
        msg->ExtId = (((sidl&0x03)<<16)|(eid8<<8)|eid0);
    else
        msg->StdId = ((sidh<<3) | (sidl>>5));
    
    msg->DLC = dlc&0x0f;
    
    MCP2515_ReadReg_Seq(REG_RXB0D0,msg->Data,msg->DLC);
}



/******************************************************
function    : static bool MCP2515_Can_Init(void)
description : MCP2515 CAN ͨ�ų�ʼ�� 
input       : void
output      : true - �ɹ� 
******************************************************/
static u8 MCP2515_Can_Init(void)
{   
    u8 sta;
    /* ���� config ģʽ */
    MCP2515_Reset();  
    MCP2515_BitModify(REG_CANCTRL,REG_BIT7,BIT_SET);
    /* ���� CAN ������/���ձ���  */
    MCP2515_Can_ConfigBaud();
    
    MCP2515_WriteReg(REG_TXRTSCTRL,0x00);   /* ʧ�� RTS �ܽ� */
    
    MCP2515_WriteReg(REG_BFPCTRL,0x00);     /* ʧ�� RX �ܽ� */
     
    MCP2515_WriteReg(REG_CANINTF,0x00);  /* ��������жϱ�� */
         
    /* ������������ģʽ */
    MCP2515_BitModify(REG_CANCTRL,REG_BIT7|REG_BIT6|REG_BIT5,BIT_RESET);
 
    return 1; 
}



/******************************************************
function    : bool MCP2515_Init(void)
description :
input       :
return      :
*******************************************************/
u8 MCP2515_Init(void)
{
    Spi1_Init();
    
    MCP2515_Can_Init();
    
    return 1;
}



/**************************************************************
function    : bool MCP2515_Can_Send_Msg(_MCP2515_CAN_MSG *msg)
description : MCP2515 can���ķ��� 
input       : *msg  �������� 
retutn      : true - �ɹ�
              false - ʧ��
****************************************************************/
u8 MCP2515_Can_Send_Msg(_MCP2515_CAN_MSG *msg)
{
    u32 i=0;
    while( (MCP2515_ReadStaReg() & 0x04) == 0x04);  /* �ȴ�BUFF0 �� */

    MCP2515_Can_ConfigMsg(msg);     /* װ��Ľ��� BUFF0 �� */
     
    MCP2515_RTS(0);     /* ������ BUFF0 */

    while( ((MCP2515_ReadReg(REG_TXB0CTRL) & 0x08) == 0x08 )&&(i++<0xffff) );    /* �ȴ�������� */
  
    if( i>0xffff)
        return 0;
    return 1;
}




/************************************************************
function    : _MCP2515_CAN_MSG *MCP_Can_Recv_Msg(void)
description : MCP2515 接收CAN 报文 
input       : void
return      : _MCP2515_CAN_MSG 数据内容
**************************************************************/
_MCP2515_CAN_MSG *MCP_Can_Recv_Msg(void)
{
    u8 STA;
    static _MCP2515_CAN_MSG msg;
    
    while( (MCP2515_ReadReg(REG_CANINTF) &  0x01) == 0x01)/* �ȴ�BUFF0���Ľ������ */
    {
        MCP2515_Can_GetMsg(&msg);
        MCP2515_BitModify(REG_CANINTF,REG_BIT0,BIT_RESET);  /* ���BUFF0���� ��־λ */
    }
    
    STA = MCP2515_ReadReg(REG_CANINTF);
    
    return &msg;
}


_MCP2515_CAN_MSG testmsg;
_MCP2515_CAN_MSG *testrecmsg;
void Test_Mcp2515_Can(void)
{
    testmsg.RTR = 0;
    testmsg.IDE = 1;
    testmsg.ExtId = 0x123;
    testmsg.StdId = 0x123;
    testmsg.DLC = 8;
    for(int i=0;i<8;i++)
        testmsg.Data[i] = i+1;
    MCP2515_Can_Send_Msg(&testmsg);
    testrecmsg = MCP_Can_Recv_Msg();
}


