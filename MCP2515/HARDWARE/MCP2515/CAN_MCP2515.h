#ifndef __CAN_MCP2515_H
#define __CAN_MCP2515_H

#include "sys.h"


#ifdef __cplusplus
extern "C" {
#endif

/* MCP2515 CAN  �������ݽṹ */
typedef struct
{
    u32 StdId;      /* ��׼֡ ID */
    u32 ExtId;      /* ��չ֡ ID */
    u8  IDE;        /* 0 : ��׼֡  1����չ֡ */
    u8  RTR;        /* 0 ������֡  1��Զ��֡ */
    u8  DLC;        /* ���ݳ��� */
    u8  Data[8];    /* �������� */
}_MCP2515_CAN_MSG;    
    
    
u8 MCP2515_Init(void);
u8 MCP2515_Can_Send_Msg(_MCP2515_CAN_MSG *msg);  
_MCP2515_CAN_MSG *MCP_Can_Recv_Msg(void);

void Test_Mcp2515_Can(void);
    
#ifdef __cplusplus
}
#endif


#endif



