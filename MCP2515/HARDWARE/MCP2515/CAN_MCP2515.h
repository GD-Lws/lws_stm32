#ifndef __CAN_MCP2515_H
#define __CAN_MCP2515_H

#include "sys.h"


#ifdef __cplusplus
extern "C" {
#endif

/* MCP2515 CAN  报文数据结构 */
typedef struct
{
    u32 StdId;      /* 标准帧 ID */
    u32 ExtId;      /* 拓展帧 ID */
    u8  IDE;        /* 0 : 标准帧  1：拓展帧 */
    u8  RTR;        /* 0 ：数据帧  1：远程帧 */
    u8  DLC;        /* 数据长度 */
    u8  Data[8];    /* 数据内容 */
}_MCP2515_CAN_MSG;    
    
    
u8 MCP2515_Init(void);
u8 MCP2515_Can_Send_Msg(_MCP2515_CAN_MSG *msg);  
_MCP2515_CAN_MSG *MCP_Can_Recv_Msg(void);

void Test_Mcp2515_Can(void);
    
#ifdef __cplusplus
}
#endif


#endif



