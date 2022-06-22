/******************************************************************************
* Company        : HCM Universyty of Technology. 
* Author         : Huy Le
* Description    : 
* Limitations    : 
******************************************************************************/

/******************************************************************************
* Include Files
******************************************************************************/
#include "comm_output_handle.h"
/******************************************************************************
* Local Variables
******************************************************************************/

/******************************************************************************
* Global Variables
******************************************************************************/

u8 Comm_TxBuff[BUFF_SIZE_TX];
/******************************************************************************
* Global Function Declarations
******************************************************************************/

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void comm_output_handle_init(void)
{
//	Comm_TxBuff[0]=0;//tranh truong hop truyen gia tri 0x00
//	Comm_TxBuff[1]=0;//tranh 0x00
//	Comm_TxBuff[2]=0;
//	Comm_TxBuff[3]=0;
//	Comm_TxBuff[4]='0';
//	Comm_TxBuff[5]='0';
//	Comm_TxBuff[6]='0';
//	Comm_TxBuff[7]='0';
//	Comm_TxBuff[8]='\r';
//	Comm_TxBuff[9]='\n';
	Comm_TxBuff[0]='A';//tranh truong hop truyen gia tri 0x00
	Comm_TxBuff[1]='T';//tranh 0x00
	Comm_TxBuff[2]='+';
	Comm_TxBuff[3]='I';
	Comm_TxBuff[4]='P';
	Comm_TxBuff[5]='A';
	Comm_TxBuff[6]='D';
	Comm_TxBuff[7]='D';
	Comm_TxBuff[8]='R';
	Comm_TxBuff[9]='\r';
	Comm_TxBuff[10]='\n';

}
void comm_output_handle_SetFrameToSend(u8* Frame_Tx)
{
//	Get_VehicleStatus();
	set_txbuff_and_send(Frame_Tx,"AT\r\n");
}
void Get_VehicleStatus(void)
{
//	speedTx=((double)vehdata_speed_data.current_dac_val/4095)*30;//tranh truong hop truyen gia tri 0x00
//	brakeTx= Rx_Frame_getBrake();
//	angleTx= Rx_Frame_getAngle();
//	modeTx=	 Rx_Frame_getMode();
//	turnTx=  Rx_Frame_getTurn();
//	fr_lightTx=  Rx_Frame_getFr_light();
//	hornTx =  Rx_Frame_getHorn();	
}

static u16 Tx_Len;
void comm_output_handler(char new_msg[])
{
//	comm_output_handle_SetFrameToSend(Comm_TxBuff);
	Tx_Len =set_txbuff_and_send(Comm_TxBuff, new_msg);
	if (uart_comm_GetTxStatus() == uart_comm_transmit_idle)
	{
		uart_comm_SendFrame(Comm_TxBuff, Tx_Len);
	}
}
u16 set_txbuff_and_send(uint8_t* txbuff, char new_msg[])
{
	int msg_send_size = strlen(new_msg);
	
	for (int i=0;i<msg_send_size;i++)
	{
		txbuff[i] = new_msg[i];
	}
	
	for (int i = msg_send_size; i< BUFF_SIZE_TX; i++)
	{
		txbuff[i] = 0;
	}

	// if ((rcv_flag == 0) & (rcv_status == 0))
	// {
	// 	DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
	// 	DMA_SetCurrDataCounter(DMA1_Stream6, msg_send_size);
	// 	DMA_Cmd(DMA1_Stream6, ENABLE);
	// 	request_response();
	// }
	return msg_send_size;
}
