/******************************************************************************
* Company        : HCM Universyty of Technology. 
* Author         : Huy Le
* Description    : 
* Limitations    : 
******************************************************************************/

#ifndef COMM_OUTPUT_HANDLE_H
#define COMM_OUTPUT_HANDLE_H

/******************************************************************************
* Include Files
******************************************************************************/
#include "stm_base.h"
#include "uart_comm.h"

/*******************************************************************************
* Definitions 
*******************************************************************************/
#define BUFF_SIZE_TX		1024

/******************************************************************************
* Macro Definitions
******************************************************************************/

/******************************************************************************
* Externs Definitions
******************************************************************************/
extern u8 Comm_TxBuff[BUFF_SIZE_TX];

extern void comm_output_handle_init(void);
extern void comm_output_handler(char new_msg[]);
extern void comm_output_handle_SetFrameToSend(u8* Frame_Tx);
extern void Get_VehicleStatus(void);
extern u16 set_txbuff_and_send(uint8_t* txbuff, char new_msg[]);

#endif /* COMM_OUTPUT_HANDLE_H */
