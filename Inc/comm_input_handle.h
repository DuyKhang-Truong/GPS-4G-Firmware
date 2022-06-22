/******************************************************************************
* Company        : HCM Universyty of Technology. 
* Author         : Huy Le
* Description    : 
* Limitations    : 
******************************************************************************/

#ifndef COMM_INPUT_HANDLE_H
#define COMM_INPUT_HANDLE_H

/******************************************************************************
* Include Files
******************************************************************************/
#include "stm_base.h"
#include "uart_comm.h"

/*******************************************************************************
* Definitions 
*******************************************************************************/
#define BUFF_SIZE_RX			1024

/******************************************************************************
* Macro Definitions
******************************************************************************/

/******************************************************************************
* Externs Definitions
******************************************************************************/ 
extern u8 Comm_RxBuff[BUFF_SIZE_RX];

extern void comm_input_handle_init(void);
extern void comm_input_handler(u32 expect_res_size);
extern u8 Process_RequestFrame(u8 * Frame,u8 Length);
extern u8 Rx_Frame_getSpeed(void);
extern u8 Rx_Frame_getBrake(void);
extern s8 Rx_Frame_getAngle(void);
extern u8 Rx_Frame_getMode(void);
extern u8 Rx_Frame_getTurn(void);
extern u8 Rx_Frame_getFr_light(void);
extern u8 Rx_Frame_getHorn(void);


#endif /* COMM_INPUT_HANDLE_H */
